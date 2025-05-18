#include "../minishell.h"

pid_t	g_signal_pid


void	data_init(t_data *data, int ac, char **av)
{

}

int main(int ac, char *av)
{
	
}
//test tokens
int main(void)
{
	char *input = "echo hello | grep h > out.txt && ls -l";
	t_token *tokens = tokenize(input);

	if (!tokens)
	{
		printf("Tokenisation échouée.\n");
		return (1);
	}

	printf("Tokens extraits :\n");
	while (tokens)
	{
		printf("Type: %d\tValue: [%s]\n", tokens->type, tokens->value);
		tokens = tokens->next;
	}
	free_token_list(tokens);

	return (0);
}