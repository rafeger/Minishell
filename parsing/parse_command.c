#include "minishell.h"

t_command *parse_command(t_token *tokens)
{
	t_command *cmd;

	cmd = malloc(sizeof(t_command));
	if (!cmd)
		return NULL;
	ft_memset(cmd, 0, sizeof(t_command));

	if (tokens->type == 1)
	{
		
	}
}