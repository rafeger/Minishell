#include "../../include/minishell.h"

/*
 * Validates redirection followed by pipe.
 * Checks for invalid sequences like >| or <|.
 * Returns 1 and displays error if invalid, 0 if valid.
*/
int	check_redir_pipe(char *input, int *i)
{
	int	j;

	j = *i + 1;
	while (input[j] && ft_isspace(input[j]))
		j++;
	if (input[j] == '|')
	{
		ft_putendl_fd("minishell: syntax error near unexpected token '|'", 2);
		return (1);
	}
	return (0);
}

/*
 * Checks for consecutive pipe characters and following content.
 * Validates that:
 * - No more than one pipe character in sequence.
 * - Content follows pipe character.
 * Returns 1 and displays error if invalid, 0 if valid.
*/
int	check_pipe_sequence(char *input, int *i)
{
	int	count;
	int	j;

	count = 1;
	while (input[*i + count] && input[*i + count] == '|')
		count++;
	if (count > 1)
	{
		ft_putendl_fd("minishell: syntax error near unexpected token '||'", 2);
		*i += count - 1;
		return (1);
	}
	j = *i + 1;
	while (input[j] && ft_isspace(input[j]))
		j++;
	if (!input[j] || input[j] == '|')
	{
		ft_putendl_fd("minishell: syntax error near unexpected token '|'", 2);
		return (1);
	}
	return (0);
}

/*
 * Verifies input doesn't start with pipe character.
 * Skips leading whitespace before check.
 * Returns 1 and displays error if pipe at start.
 * Returns 0 if start is valid.
*/
int	check_pipe_start(char *input)
{
	int	i;

	i = 0;
	while (input[i] && ft_isspace(input[i]))
		i++;
	if (input[i] == '|')
	{
		if (input[i + 1] == '|')
		{
			ft_putstr_fd("minishell: ", 2);
			ft_putendl_fd("syntax error near unexpected token '||'", 2);
		}
		else
		{
			ft_putstr_fd("minishell: ", 2);
			ft_putendl_fd("syntax error near unexpected token '|'", 2);
		}
		return (1);
	}
	return (0);
}

/*
 * Main pipe syntax validation function.
 * Checks:
 * - Valid start.
 * - Proper pipe sequences throughout input.
 * Returns 1 if any pipe syntax error found, 0 if valid.
*/
int	check_pipe_syntax(char *input)
{
	int	i;

	if (check_pipe_start(input))
		return (1);
	i = 0;
	while (input[i])
	{
		if (input[i] == '|')
		{
			if (check_pipe_sequence(input, &i))
				return (1);
		}
		i++;
	}
	return (0);
}
