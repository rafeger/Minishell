
#include "../../include/minishell.h"


int	check_multi_redir(char *input, int *i)
{
	int	count;

	count = 1;
	while (input[*i + count] && (input[*i + count] == input[*i]))
		count++;
	if (count > 2)
	{
		if (count > 3)
		{
			ft_putstr_fd("minishell: ", 2);
			if (input[*i] == '>')
				ft_putendl_fd("syntax error near unexpected token '>>'", 2);
			else
				ft_putendl_fd("syntax error near unexpected token '<<'", 2);
			return (1);
		}
		ft_putstr_fd("minishell: ", 2);
		if (input[*i] == '>')
			ft_putendl_fd("syntax error near unexpected token '>'", 2);
		else
			ft_putendl_fd("syntax error near unexpected token '<'", 2);
		return (1);
	}
	*i += count - 1;
	return (0);
}

int	check_newline_after_redir(char *input, int i)
{
	while (input[i] && ft_isspace(input[i]))
		i++;
	if (!input[i])
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putendl_fd("syntax error near unexpected token 'newline'", 2);
		return (1);
	}
	return (0);
}

int	check_consecutive_redir(char *input, int *i)
{
	int		j;
	char	first;
	char	second;

	first = input[*i];
	j = *i + 1;
	while (input[j] && ft_isspace(input[j]))
		j++;
	if (input[j] && (input[j] == '<' || input[j] == '>'))
	{
		second = input[j];
		ft_putstr_fd("minishell: syntax error near unexpected token '", 2);
		if (first == second)
		{
			ft_putchar_fd(second, 2);
			ft_putchar_fd(first, 2);
		}
		else
			ft_putchar_fd(second, 2);
		ft_putendl_fd("'", 2);
		return (1);
	}
	return (0);
}

int	check_redirections(char *input, int *i)
{
	if (check_if_quotes(input, i))
		return (0);
	if (check_redir_pipe(input, i))
		return (1);
	if (check_multi_redir(input, i))
		return (1);
	if (check_newline_after_redir(input, *i + 1))
		return (1);
	if (check_consecutive_redir(input, i))
		return (1);
	if (check_redir_pipe(input, i))
		return (1);
	return (0);
}

int	check_syntax(char *input)
{
	int	i;

	if (!input)
		return (1);
	if (check_pipe_syntax(input))
		return (1);
	i = 0;
	while (input[i])
	{
		if (input[i] == '>' || input[i] == '<')
		{
			if (check_redirections(input, &i))
				return (1);
		}
		i++;
	}
	return (0);
}
