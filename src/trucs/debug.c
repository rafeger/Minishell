#include "../../minishell.h"

void print_tokens(t_token *tok)
{
	while (tok)
	{
		printf("Token: %-10s | Type: %d\n", tok->value, tok->type);
		tok = tok->next;
	}
}

void print_commands(t_command *cmd)
{
	while (cmd)
	{
		printf("Command:\n");
		for (int i = 0; cmd->args && cmd->args[i]; i++)
			printf("  Arg[%d]: %s\n", i, cmd->args[i]);
		printf("  Infile: %d\n", cmd->infile);
		printf("  Outfile: %d\n", cmd->outfile);
		printf("  Append: %d\n", cmd->append);
		if (cmd->heredoc_delim != NULL)
			printf("  Heredoc: %s\n", cmd->heredoc_delim);
		cmd = cmd->next;
	}
}