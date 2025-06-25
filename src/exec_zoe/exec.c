#include "../../include/minishell.h"

pid_t g_signal;

static void close_heredoc_fds(t_cmd *cmd_list)
{
    t_cmd *cmd = cmd_list;
    
    while (cmd) {
        if (cmd->heredoc_fd != -1) {
            close(cmd->heredoc_fd);
            cmd->heredoc_fd = -1;
        }
        cmd = cmd->next;
    }
}

int is_builtin_no_fork(char *cmd)
{
    if (!ft_strcmp(cmd, "cd") || !ft_strcmp(cmd, "export")
        || !ft_strcmp(cmd, "unset") || !ft_strcmp(cmd, "exit"))
        return (1);
    return (0);
}

static void cleanup_split(char **array)
{
    int i;
    
    i = 0;
    if (!array)
        return ;
    while (array[i])
        free(array[i++]);
    free(array);
}

static void execute(t_shell_data *data, t_cmd *cmd)
{
    char *pathname;
    char **commande;
    char **tab_env;
    
    commande = cmd->args;
    pathname = get_pathname(commande[0], data->env);
    if (!pathname)
    {
        perror(commande[0]);
        exit(127);
    }
    
    if (access(pathname, X_OK) != 0)
    {
        perror(pathname);
        free(pathname);
        exit(126);
    }
    
    tab_env = convert_list_to_tab_str(data->env);
    if (execve(pathname, commande, tab_env) == -1)
    {
        perror("execve");
        free(pathname);
        cleanup_split(tab_env);
        exit(EXIT_FAILURE);
    }
}

static void setup_pipes_and_redirections(t_cmd *cmd, int pipe_in, int pipe_out)
{
	(void)cmd;
    // Setup input
    if (pipe_in != STDIN_FILENO)
    {
        dup2(pipe_in, STDIN_FILENO);
        close(pipe_in);
    }
    
    // Setup output
    if (pipe_out != STDOUT_FILENO)
    {
        dup2(pipe_out, STDOUT_FILENO);
        close(pipe_out);
    }
}

static void child_process(t_shell_data *data, t_cmd *cmd, int pipe_in, int pipe_out)
{
    int exit_status;

    setup_pipes_and_redirections(cmd, pipe_in, pipe_out);
    
    // Apply file redirections (< > >> <<)
    redirections(data, cmd);
    
    if (is_builtin(cmd->args[0]))
    {
        exit_status = do_builtin(data, cmd);
        ft_cleanup_shell(&data);
		rl_clear_history();
        exit(exit_status);
    }
    else
    {
        execute(data, cmd);
    }
}

int execute_commands(t_shell_data *data)
{
    t_cmd *cmd;
    int pipe_fd[2];
    int input_fd = STDIN_FILENO;
    pid_t pid;
    int status;
    
    cmd = data->cmd;
    
    // Handle single builtin command that doesn't need fork
    if (!cmd->next && is_builtin_no_fork(cmd->args[0])) {
        redirections(data, cmd);
        do_builtin(data, cmd);
        // Close heredoc FDs after builtin execution
        close_heredoc_fds(data->cmd);
        return (0);
    }
    
    while (cmd) {
        int output_fd = STDOUT_FILENO;
        
        // If there's a next command, create a pipe
        if (cmd->next) {
            if (pipe(pipe_fd) == -1) {
                perror("pipe");
                // Close any remaining heredoc FDs before returning
                close_heredoc_fds(data->cmd);
                return (1);
            }
            output_fd = pipe_fd[1];
        }
        
        pid = fork();
        if (pid == -1) {
            perror("fork");
            // Close pipe FDs and heredoc FDs
            if (cmd->next) {
                close(pipe_fd[0]);
                close(pipe_fd[1]);
            }
            close_heredoc_fds(data->cmd);
            return (1);
        }
        
        if (pid == 0) {
            // Child process
            if (cmd->next)
                close(pipe_fd[0]); // Close unused read end
            child_process(data, cmd, input_fd, output_fd);
            // Never reached
        } else {
            // Parent process - close heredoc FD for this command after fork
            if (cmd->heredoc_fd != -1) {
                close(cmd->heredoc_fd);
                cmd->heredoc_fd = -1;
            }
            
            if (input_fd != STDIN_FILENO)
                close(input_fd); // Close previous pipe read end
                
            if (cmd->next) {
                close(pipe_fd[1]); // Close write end
                input_fd = pipe_fd[0]; // Save read end for next command
            }
            
            // If this is the last command, wait for it
            if (!cmd->next) {
                waitpid(pid, &status, 0);
                if (WIFEXITED(status))
                    data->last_exit_status = WEXITSTATUS(status);
                else if (WIFSIGNALED(status))
                    data->last_exit_status = 128 + WTERMSIG(status);
            }
        }
        cmd = cmd->next;
    }
    
    // Wait for any remaining child processes
    while (wait(NULL) > 0)
        ; // Wait for all children to finish
        
    // Close any remaining input fd
    if (input_fd != STDIN_FILENO)
        close(input_fd);
        
    // Ensure all heredoc FDs are closed
    close_heredoc_fds(data->cmd);
    
    return (0);
}