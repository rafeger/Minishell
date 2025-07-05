#include "../../include/minishell.h"

int	redir_overflow(const char *line, int pos)
{
	int	n;

	n = 1;
	while (line[pos + n] && line[pos + n] == line[pos])
		n++;
	if (n > 2)
	{
		ft_putstr_fd("minishell: ", 2);
		if (line[pos] == '>')
		{
			if (n > 3)
				ft_putendl_fd("syntax error near unexpected token '>>'", 2);
			else
				ft_putendl_fd("syntax error near unexpected token '>'", 2);
		}
		else
		{
			if (n > 3)
				ft_putendl_fd("syntax error near unexpected token '<<'", 2);
			else
				ft_putendl_fd("syntax error near unexpected token '<'", 2);
		}
		return (1);
	}
	return (0);
}

int	redir_missing_target(const char *line, int pos)
{
	int	next;

	next = skip_blanks(line, pos + 1);
	if (!line[next])
	{
		print_newline_error();
		return (1);
	}
	if ((line[next] == '<' || line[next] == '>') && 
		!(line[next] == line[pos] && next == pos + 1))
	{
		print_syntax_error_char(line[next]);
		return (1);
	}
	return (0);
}

int	redir_invalid_combo(const char *line, int pos)
{
	int		j;
	char	first;
	char	second;

	j = pos + 1;
	first = line[pos];
	while (line[j] && ft_isspace(line[j]))
		j++;
	if (line[j] && (line[j] == '<' || line[j] == '>'))
	{
		if (line[j] == first && j == pos + 1)
			return (0);
		second = line[j];
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

int	redir_pipe_error(const char *line, int pos)
{
	int	next;

	next = pos + 1;
	while (line[next] && ft_isspace(line[next]))
		next++;
	if (line[next] == '|')
	{
		print_syntax_error("|");
		return (1);
	}
	return (0);
}
