#include "../../minishell.h"

bool ft_isspace(char c)
{
	if (c == ' ' || c == '\t')
		return (true);
	return (false);
}

void	handle_whitespaces(char *str, int *i)
{
	while (str[*i] == ' ' || str[*i] == '\t')
		(*i)++;
}

