#include "../../include/minishell.h"

int	check_redir_pipe(char *input, int *i)
{
	int	next_char_idx;

	next_char_idx = *i + 1;
	while (input[next_char_idx] && ft_isspace(input[next_char_idx]))
		next_char_idx++;
	if (input[next_char_idx] == '|')
	{
		ft_putendl_fd("minishell: syntax error near unexpected token '|'", 2);
		return (1);
	}
	return (0);
}

int	check_pipe_sequence(char *input, int *i)
{
	int	pipe_count;
	int	trailing_char_idx;

	pipe_count = 1;
	while (input[*i + pipe_count] && input[*i + pipe_count] == '|')
		pipe_count++;
	if (pipe_count > 1)
	{
		ft_putendl_fd("minishell: syntax error near unexpected token '||'", 2);
		*i += pipe_count - 1;
		return (1);
	}
	trailing_char_idx = *i + 1;
	while (input[trailing_char_idx] && ft_isspace(input[trailing_char_idx]))
		trailing_char_idx++;
	if (!input[trailing_char_idx] || input[trailing_char_idx] == '|')
	{
		ft_putendl_fd("minishell: syntax error near unexpected token '|'", 2);
		return (1);
	}
	return (0);
}

int	check_pipe_start(char *input)
{
	int	current_idx;

	current_idx = 0;
	while (input[current_idx] && ft_isspace(input[current_idx]))
		current_idx++;
	if (input[current_idx] == '|')
	{
		ft_putstr_fd("minishell: ", 2);
		if (input[current_idx + 1] == '|')
			ft_putendl_fd("syntax error near unexpected token '||'", 2);
		else
			ft_putendl_fd("syntax error near unexpected token '|'", 2);
		return (1);
	}
	return (0);
}

int	check_pipe_syntax(char *input)
{
	int	current_pos;

	if (check_pipe_start(input))
		return (1);
	current_pos = 0;
	while (input[current_pos])
	{
		if (input[current_pos] == '|')
		{
			if (check_pipe_sequence(input, &current_pos))
				return (1);
		}
		current_pos++;
	}
	return (0);
}