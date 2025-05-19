#include "../../minishell.h"

t_command *create_command(void)
{
	t_command *cmd;
	cmd = calloc(1, sizeof(t_command));\
	if (!cmd)
		return NULL;
	cmd->infile = -1;
	cmd->outfile = -1;
	return cmd;
}

