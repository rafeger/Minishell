#include "../../include/minishell.h"

int	pipe_at_begin(const char *line)
{
	int	idx;

	idx = skip_blanks(line, 0);
	if (line[idx] == '|')
	{
		ft_putstr_fd("minishell: ", 2);
		if (line[idx + 1] == '|')
			print_syntax_error("||");
		else
			print_syntax_error("|");
		return (1);
	}
	return (0);
}

int	pipe_sequence_error(const char *line, int *idx)
{
	int	count;
	int	trailing;

	count = 1;
	trailing = *idx + 1;
	while (line[*idx + count] && line[*idx + count] == '|')
		count++;
	if (count > 1)
	{
		print_syntax_error("||");
		*idx += count - 1;
		return (1);
	}
	while (line[trailing] && ft_isspace(line[trailing]))
		trailing++;
	if (!line[trailing] || line[trailing] == '|')
	{
		print_syntax_error("|");
		return (1);
	}
	return (0);
}

int	check_pipe_syntax(const char *line, int k)
{
	if (line[k] == '|' && !is_it_in_q(line, k))
	{
		if (pipe_sequence_error(line, &k))
			return (1);
	}
	return (0);
}
