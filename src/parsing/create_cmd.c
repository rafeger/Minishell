#include "../../minishell.h"



t_command	*parse_tokens(t_token *tokens, char **envp)
{
	t_command	*head = NULL;
	t_command	*current = NULL;

	while (tokens)
	{
		t_command *cmd = malloc(sizeof(t_command));
		if (!cmd)
			return (free_command_list(head), NULL);
		init_command(cmd);
		if (!fill_command(cmd, &tokens, envp))
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

char	*expand_variables(const char *str, char **envp)
{
	char	*result = ft_strdup("");
	int		i = 0;

	while (str[i])
	{
		if (str[i] == '$' && str[i + 1] && (ft_isalpha(str[i + 1]) || str[i + 1] == '_'))
		{
			int	start = ++i;
			while (str[i] && (ft_isalnum(str[i]) || str[i] == '_'))
				i++;
			char *var = ft_substr(str, start, i - start);
			char *val = getenv_from_envp(var, envp);
			free(var);
			if (val)
			{
				char *tmp = result;
				result = ft_strjoin(result, val);
				free(tmp);
			}
		}
		else
		{
			char buf[2] = {str[i++], 0};
			char *tmp = result;
			result = ft_strjoin(result, buf);
			free(tmp);
		}
	}
	return (result);
}

