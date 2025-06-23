#include "../../include/minishell.h"

static void input_redirection(t_shell_data *data, t_redirect *redir)
{
	(void)data;
    int fd;
    
    fd = open(redir->file, O_RDONLY);
    if (fd == -1)
    {
        perror("open");
        exit(1);
    }
    if (dup2(fd, STDIN_FILENO) == -1)
    {
        perror("dup2");
        close(fd);
        exit(1);
    }
    close(fd);
}

static void output_redirection(t_shell_data *data, t_redirect *redir)
{
	(void)data;
    int fd;
    
    fd = open(redir->file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd == -1)
    {
        perror("open");
        exit(1);
    }
    if (dup2(fd, STDOUT_FILENO) == -1)
    {
        perror("dup2");
        close(fd);
        exit(1);
    }
    close(fd);
}

static void append_redirection(t_shell_data *data, t_redirect *redir)
{
	(void)data;
    int fd;
    
    fd = open(redir->file, O_WRONLY | O_CREAT | O_APPEND, 0644);
    if (fd == -1)
    {
        perror("open");
        exit(1);
    }
    if (dup2(fd, STDOUT_FILENO) == -1)
    {
        perror("dup2");
        close(fd);
        exit(1);
    }
    close(fd);
}

static void here_document(t_cmd *cmd)
{
    if (cmd->heredoc_fd == -1)
        return;
        
    if (dup2(cmd->heredoc_fd, STDIN_FILENO) == -1)
    {
        perror("dup2");
        exit(1);
    }
    close(cmd->heredoc_fd);
}

void redirections(t_shell_data *data, t_cmd *cmd)
{
    t_redirect *redir;
    
    redir = cmd->redirects;
    while (redir)
    {
        if (redir->type == REDIR_IN)
            input_redirection(data, redir);
        else if (redir->type == REDIR_OUT)
            output_redirection(data, redir);
        else if (redir->type == APPEND)
            append_redirection(data, redir);
        else if (redir->type == HERE_DOC)
            here_document(cmd);
        redir = redir->next;
    }
}