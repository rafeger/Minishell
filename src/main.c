#include "../minishell.h"

void	init_command(t_command *cmd)
{
	cmd->args = NULL;
	cmd->infile = -1;
	cmd->outfile = -1;
	cmd->append = 0;
	cmd->heredoc_delim = NULL;
	cmd->next = NULL;
}





int	main(int argc, char **argv, char **envp)
{
	char	*line;
	t_token	*tokens;
	t_command *commands;

	(void)argc;
	(void)argv;
	(void)envp;

	while (1)
	{
		line = readline("minishell$ ");
		if (!line)
			break;

		if (*line)
			add_history(line);

		tokens = tokenize(line);
		if (!tokens)
		{
			free(line);
			continue;
		}

		printf("\n--- Tokens ---\n");
		print_tokens(tokens);

		commands = parse_tokens(tokens);
		if (commands)
		{
			printf("\n--- Parsed Commands ---\n");
			print_commands(commands);
			free_command_list(commands);
		}

		free_token_list(tokens);
		free(line);
	}
	return (0);
}