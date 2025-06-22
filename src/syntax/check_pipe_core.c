#include "../../include/minishell.h"


/**
 * @brief Validates if a redirection operator is immediately followed by a pipe.
 *
 * This function checks for invalid syntax patterns where a redirection symbol
 * (like '>' or '<') is directly succeeded by a pipe symbol ('|'), optionally
 * with whitespace in between. Such sequences are syntactically incorrect
 * in shell commands.
 *
 * @param input The entire command line input string.
 * @param i A pointer to the current index in the input string, pointing to the redirection operator.
 * @return Returns 1 if an invalid redirection-pipe sequence is found and prints an error message.
 * @return Returns 0 if the sequence is valid or no such sequence is present.
 */
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

/**
 * @brief Examines a sequence of pipe characters for syntax correctness.
 *
 * This function ensures that there are no more than one consecutive pipe
 * characters (e.g., "||" is an error) and that a pipe character is always
 * followed by some command or argument, not immediately another pipe or end of string.
 * It helps catch common pipe-related syntax errors.
 *
 * @param input The entire command line input string.
 * @param i A pointer to the current index in the input string, pointing to a pipe character.
 * @return Returns 1 if an invalid pipe sequence is found and prints an error message.
 * @return Returns 0 if the pipe sequence is syntactically valid.
 */
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

/**
 * @brief Checks if the command input begins with a pipe character after skipping leading whitespace.
 *
 * A command line should not start with a pipe symbol, as it implies an
 * unnamed input source which is typically invalid. This function identifies
 * and reports such syntax errors.
 *
 * @param input The command line input string.
 * @return Returns 1 if the input string starts with a pipe (after trimming whitespace) and prints an error.
 * @return Returns 0 if the input does not start with a pipe.
 */
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

/**
 * @brief Performs a comprehensive syntax check specifically for pipe characters within the input.
 *
 * This is the main entry point for validating pipe-related syntax. It first
 * verifies that the input does not begin with a pipe. Then, it iterates
 * through the entire input string, checking for correctly formed pipe sequences
 * wherever a pipe character is encountered.
 *
 * @param input The complete command line string to be validated.
 * @return Returns 1 if any pipe syntax error is detected.
 * @return Returns 0 if all pipe syntax within the input is valid.
 */
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