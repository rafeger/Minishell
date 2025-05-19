#include "../minishell.h"

void	print_tokens(t_token *tok)
{
	while (tok)
	{
		printf("TOKEN: [%s] | TYPE: %d\n", tok->value, tok->type);
		tok = tok->next;
	}
}
void	init_command(t_command *cmd)
{
	cmd->args = NULL;
	cmd->infile = -1;
	cmd->outfile = -1;
	cmd->append = 0;
	cmd->heredoc_delim = NULL;
	cmd->next = NULL;
}



int	main(void)
{
	char	input[1024];
	t_token	*tokens;

	while (1)
	{
		printf("minishell> ");
		if (!fgets(input, sizeof(input), stdin))
			break;
		if (input[0] == '\0')
			continue;
		input[strcspn(input, "\n")] = '\0';


		tokens = tokenize(input);
		if (!tokens)
		{
			printf("Tokenization failed.\n");
			continue;
		}
		print_tokens(tokens);

		if (check_syntax_errors(tokens))
			printf("Syntax error detected.\n");
		else
			printf("No syntax errors.\n");

		free_token_list(tokens);
	}
	return (0);
}