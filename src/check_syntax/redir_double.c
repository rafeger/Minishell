#include "../../include/minishell.h"

static int	check_double_redir_target(const char *line, int k)
{
	int	next;

	next = skip_blanks(line, k + 2);
	if (!line[next])
	{
		print_newline_error();
		return (1);
	}
	if ((line[next] == '<' || line[next] == '>'))
	{
		print_syntax_error_char(line[next]);
		return (1);
	}
	return (0);
}

int	handle_double_redirection(const char *line, int *k)
{
	if (line[*k] == line[*k + 1] && !is_it_in_q(line, *k + 1))
	{
		if (redir_pipe_error(line, *k))
			return (1);
		if (redir_overflow(line, *k))
			return (1);
		if (check_double_redir_target(line, *k))
			return (1);
		
		*k += 2;
		while (line[*k] && ft_isspace(line[*k]))
			(*k)++;
		return (0);
	}
	return (-1);
}

int	check_single_redirection(const char *line, int k)
{
	if (redir_pipe_error(line, k))
		return (1);
	if (redir_overflow(line, k))
		return (1);
	if (redir_missing_target(line, k))
		return (1);
	if (redir_invalid_combo(line, k))
		return (1);
	return (0);
}
