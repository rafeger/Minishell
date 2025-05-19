#include "../../minishell.h"

void	free_command_list(t_command *cmd)
{
	t_command	*tmp;
	int			i;

	while (cmd)
	{
		tmp = cmd;
		i = 0;
		while (tmp->args && tmp->args[i])
		{
			free(tmp->args[i]);
			i++;
		}
		free(tmp->args);
		if (tmp->infile > 0)
			close(tmp->infile);
		if (tmp->outfile > 0)
			close(tmp->outfile);
		cmd = cmd->next;
		free(tmp);
	}
}

void	free_token_list(t_token *tok)
{
	t_token	*tmp;

	while (tok)
	{
		tmp = tok->next;
		free(tok->value);
		free(tok);
		tok = tmp;
	}
}
