#include "../../include/minishell.h"

static int	check_redirection_syntax(const char *line, int *k)
{
	int	result;

	if ((line[*k] == '>' || line[*k] == '<') && !is_it_in_q(line, *k))
	{
		result = handle_double_redirection(line, k);
		if (result == 1)
			return (1);
		if (result == 0)
			return (0);
		if (check_single_redirection(line, *k))
			return (1);
	}
	return (0);
}

int	shell_syntax_check(const char *line)
{
	int	k;

	k = 0;
	if (!line)
		return (1);
	if (pipe_at_begin(line))
		return (1);
	
	while (line[k])
	{
		if (check_pipe_syntax(line, k))
			return (1);
		
		if (check_redirection_syntax(line, &k))
			return (1);
		
		k++;
	}
	return (0);
}
