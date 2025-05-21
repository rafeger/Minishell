#include "../../minishell.h"

t_command	*parse_tokens(t_token *tokens)
{
	t_command	*head = NULL;
	t_command	*current = NULL;

	while (tokens)
	{
		t_command *cmd = malloc(sizeof(t_command));
		if (!cmd)
			return (free_command_list(head), NULL);
		init_command(cmd);
		if (!fill_command(cmd, &tokens))
		{
			free(cmd);
			return (free_command_list(head), NULL);
		}
		if (!head)
			head = cmd;
		else
			current->next = cmd;
		current = cmd;
		if (tokens && tokens->type == T_PIPE)
			tokens = tokens->next;
	}
	return (head);
}


int	handle_redirection(t_command *cmd, t_token **tok)
{
	t_token	*redir;
	char *filename;

	redir = *tok;
	if (!redir || !redir->next || redir->next->type != T_WORD)
		return (print_syntax_error("Missing filename for redirection\n"), 0);
	filename = redir->next->value;
	if (redir->type == T_REDIR_IN)
		cmd->infile = open(filename, O_RDONLY);
	else if (redir->type == T_REDIR_OUT)
		cmd->outfile = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	else if (redir->type == T_REDIR_APPEND)
	{
		cmd->outfile = open(filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
		cmd->append = 1;
	}
	else if (redir->type == T_REDIR_HEREDOC)
		cmd->heredoc_delim = ft_strdup(filename);
	if ((cmd->infile == -1 && redir->type == T_REDIR_IN)
		|| (cmd->outfile == -1 && redir->type != T_REDIR_IN))
	{
		perror(filename);
		return (0);
	}
	*tok = redir->next;
	return (1);
}


