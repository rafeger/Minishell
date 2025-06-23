/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zmurie <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 14:39:34 by zmurie            #+#    #+#             */
/*   Updated: 2025/04/01 14:39:36 by zmurie           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../../include/minishell.h"

pid_t g_signal;

// Fixed: renamed to better reflect its purpose
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
        perror(commande[0]); // Fixed: use command name for error
        data->last_exit_status = 127;
        exit(127); // Fixed: child must exit
    }
    
    if (access(pathname, X_OK) != 0)
    {
        perror(pathname);
        data->last_exit_status = 126;
        free(pathname); // Fixed: free pathname before exit
        exit(126); // Fixed: child must exit
    }
    
    tab_env = convert_list_to_tab_str(data->env);
    if (execve(pathname, commande, tab_env) == -1)
    {
        perror("execve");
        free(pathname);
        cleanup_split(tab_env); // Fixed: free tab_env
        exit(EXIT_FAILURE);
    }
    // Note: This line is never reached if execve succeeds
}

static void setup_child_fds(t_cmd *cmd, int *pipefd, int has_pipe)
{
    // Handle input redirection
    if (cmd->fd_info && cmd->fd_info->stdin_backup >= 0)
    {
        dup2(cmd->fd_info->stdin_backup, STDIN_FILENO);
        close(cmd->fd_info->stdin_backup);
    }
    
    // Handle output redirection
    if (cmd->fd_info && cmd->fd_info->stdout_backup >= 0)
    {
        dup2(cmd->fd_info->stdout_backup, STDOUT_FILENO);
        close(cmd->fd_info->stdout_backup);
    }
    else if (cmd->fd_info && cmd->fd_info->stdout_backup == -2 && has_pipe)
    {
        // Pipe output to next command
        dup2(pipefd[1], STDOUT_FILENO);
    }
    
    // Close pipe fds only if pipe was created
    if (has_pipe)
    {
        close(pipefd[0]);
        close(pipefd[1]);
    }
}

static void child_process(t_shell_data *data, t_cmd *cmd, int *pipefd, int has_pipe)
{
    setup_child_fds(cmd, pipefd, has_pipe);
    
    // Apply redirections
    redirections(data, cmd);
    
    if (is_builtin(cmd->args[0]))
    {
        do_builtin(data, cmd);
        exit(data->last_exit_status); // Fixed: builtin must exit in child
    }
    else
    {
        execute(data, cmd);
        // execute() will exit, so this line is never reached
    }
}

static void parent_process(t_cmd *cmd, int *pipefd, int has_pipe)
{
    // Only handle pipe if it was created
    if (has_pipe)
    {
        // Close write end of pipe
        close(pipefd[1]);
        
        // Set up pipe for next command if needed
        if (cmd->fd_info && cmd->fd_info->stdin_backup == -2)
        {
            cmd->fd_info->stdin_backup = pipefd[0];
        }
        else if (cmd->next && cmd->next->fd_info && cmd->next->fd_info->stdin_backup == -2)
        {
            cmd->next->fd_info->stdin_backup = pipefd[0];
        }
        else
        {
            close(pipefd[0]); // Close read end if not needed
        }
    }
    
    // Handle stdin backup cleanup
    if (cmd->fd_info && cmd->fd_info->stdin_backup >= 0)
        close(cmd->fd_info->stdin_backup);
}

int execute_commands(t_shell_data *data)
{
    int pipefd[2];
    t_cmd *tmp_cmd;
    int has_pipe;
    
    tmp_cmd = data->cmd;
    
    // Single command that doesn't need forking
    if (tmp_cmd->next == NULL && is_builtin_no_fork(tmp_cmd->args[0]))
    {
        do_builtin(data, tmp_cmd);
        return (0);
    }
    
    while (tmp_cmd)
    {
        // Determine if we need a pipe for this command
        has_pipe = (tmp_cmd->next != NULL);
        
        // Only create pipe if there's a next command
        if (has_pipe && pipe(pipefd) == -1)
        {
            perror("pipe");
            return (1);
        }
        
        g_signal = fork();
        if (g_signal == -1)
        {
            perror("fork");
            if (has_pipe)
            {
                close(pipefd[0]);
                close(pipefd[1]);
            }
            return (1);
        }
        
        if (g_signal == 0)
        {
            // Child process
            child_process(data, tmp_cmd, pipefd, has_pipe);
            // Should never reach here
        }
        else
        {
            // Parent process
            int status;
            waitpid(g_signal, &status, 0);
            
            if (WIFEXITED(status))
                data->last_exit_status = WEXITSTATUS(status);
            else if (WIFSIGNALED(status))
                data->last_exit_status = 128 + WTERMSIG(status);
            
            parent_process(tmp_cmd, pipefd, has_pipe);
        }
        
        tmp_cmd = tmp_cmd->next;
    }
    
    return (0);
}