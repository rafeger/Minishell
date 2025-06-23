#include "../../include/minishell.h"

/*
 * Sets up command structure for pipe segment.
 * Creates new token array and command structure.
 * Manages quote status and token copying.
 * Returns initialized command structure or NULL on failure.
*/
static t_cmd	*setup_pipe_cmd(t_ta *new_ta, t_ta *ta, int idx, char **stok)
{
	t_cmd	*next_cmd;

	new_ta->tokens = stok;
	if (!init_quoted_array(new_ta, ta, idx))
	{
		cleanup_pipe_data(new_ta, stok, new_ta->t_tot);
		return (NULL);
	}
	next_cmd = parse_tokens(new_ta);
	free_tokenarray(new_ta);
	return (next_cmd);
}

/**
 * @brief Processes a pipe token in the command sequence.
 *
 * When a pipe character ('|') is found, this function is responsible for
 * creating a new command structure for the command that follows the pipe.
 * It links the current command to this new command, effectively building
 * the pipeline. If the `handle_pipe` operation fails, it cleans up the
 * current command and signals failure.
 *
 * @param cmd A pointer to the current command structure, which will be linked to the next.
 * @param ta A pointer to the token array.
 * @param i A pointer to the current index in the token array, pointing to the pipe.
 * @return Returns 0 if `handle_pipe` fails, 1 on successful handling of the pipe.
 */
/*
 * Processes pipe token in parsing.
 * Creates and links new command structure.
 * Sets up command relationship in pipeline.
 * Returns 0 on failure, 1 on success.
*/

int	handle_pipe(t_cmd *cmd, t_ta *ta, int index)
{
	t_ta	*new_ta;
	t_cmd	*next_cmd;
	char	**sub_tokens;

	if (index + 1 >= ta->t_tot)
		return (0);
	new_ta = init_new_ta(ta, index);
	if (!new_ta)
		return (0);
	sub_tokens = create_sub_tokens(ta, index, new_ta);
	if (!sub_tokens)
	{
		cleanup_pipe_data(new_ta, NULL, 0);
		return (0);
	}
	next_cmd = setup_pipe_cmd(new_ta, ta, index, sub_tokens);
	if (!next_cmd)
		return (0);
	next_cmd->prev = cmd;
	cmd->next = next_cmd;
	return (1);
}

static int	handle_pipe_token(t_cmd *cmd, t_ta *ta, int *i)
{
	if (ft_strcmp(ta->tokens[*i], "|") == 0 && !ta->quoted[*i])
	{
		if (!handle_pipe(cmd, ta, *i))
		{
			free_command(cmd); 
			return (0);
		}
		*i = ta->t_tot; 
	}
	return (1);
}


/**
 * @brief Handles redirection tokens and their associated file names.
 *
 * This function is invoked when a redirection operator (`<`, `>`, `<<`, `>>`)
 * is encountered. It determines the type of redirection, then finds the
 * subsequent token which represents the target filename (skipping any
 * intervening whitespace tokens). Finally, it creates and adds a new
 * redirection structure to the current command.
 *
 * @param cmd A pointer to the current command structure.
 * @param ta A pointer to the token array.
 * @param i A pointer to the current index in the token array, pointing to the redirection operator.
 */
static void	handle_redirect(t_cmd *cmd, t_ta *ta, int *i)
{
	int	redir_kind;
	int	file_token_idx;

	redir_kind = get_redirect_type(ta->tokens[*i]);
	if (redir_kind >= 0)
	{
		file_token_idx = *i + 1;
		while (file_token_idx < ta->t_tot && ft_strcmp(ta->tokens[file_token_idx], " ") == 0)
			file_token_idx++;
		if (file_token_idx < ta->t_tot && ta->tokens[file_token_idx])
		{
			add_redirect(cmd, redir_kind, ta->tokens[file_token_idx], ta->quoted[file_token_idx]);
			*i = file_token_idx + 1; 
		}
		else
			(*i)++;
	}
}

/**
 * @brief Processes empty tokens, particularly those resulting from unquoted spaces.
 *
 * This function handles scenarios where an empty token (e.g., from `""` or `''`)
 * is encountered. It specifically deals with empty tokens that might be
 * followed by a space, ensuring they are correctly added as arguments.
 *
 * @param cmd A pointer to the current command structure.
 * @param ta A pointer to the token array.
 * @param i A pointer to the current index in the token array, pointing to the empty token.
 */
static void	handle_empty_token(t_cmd *cmd, t_ta *ta, int *i)
{
	char	*space_content;

    if ((*i + 1) < ta->t_tot && ft_strcmp(ta->tokens[*i + 1], " ") == 0 && !ta->quoted[*i + 1])
    {
        space_content = ft_strdup(ta->tokens[*i + 1]);
        if (space_content)
        {
            add_argument(cmd, space_content, ta->quoted[*i + 1]); 
            free(space_content);
        }
    }
    else
    {
        add_argument(cmd, "", ta->quoted[*i]);
	}
}

/**
 * @brief Processes a single token during the parsing phase.
 *
 * This function acts as a dispatcher, handling different types of tokens
 * encountered while parsing a command line. It determines if the token is
 * a pipe, a redirection, an empty token, or a regular argument, and calls
 * the appropriate handler function. It ensures that the command structure
 * is built correctly based on the token's type.
 *
 * @param cmd A pointer to the current command structure being built.
 * @param ta A pointer to the token array structure, containing all tokens and their quoted status.
 * @param i A pointer to the current index in the token array, which will be advanced by this function.
 * @return Returns 0 on failure during pipe handling, 1 on success for all other token types.
 */
static int	process_token(t_cmd *cmd, t_ta *ta, int *i)
{
	if (!handle_pipe_token(cmd, ta, i))
		return (0);
	if (*i >= ta->t_tot)
		return (1);
	if (ft_strcmp(ta->tokens[*i], " ") == 0 && !ta->quoted[*i] && !cmd->name)
	{
		(*i)++; 
		return (1);
	}
	if (get_redirect_type(ta->tokens[*i]) >= 0 && !ta->quoted[*i])
	{
		handle_redirect(cmd, ta, i);
	}
	else if (ft_strcmp(ta->tokens[*i], "") == 0)
	{
		handle_empty_token(cmd, ta, i);
        (*i)++; 
	}
	else
	{
		add_argument(cmd, ta->tokens[*i], ta->quoted[*i]);
		(*i)++; 
	}
	return (1);
}

/**
 * @brief Main function to parse a token array into a command structure.
 *
 * This is the primary parsing routine. It iterates through an array of tokens
 * generated by the lexer, constructs `t_cmd` structures, handles arguments,
 * redirections, and pipelines. It skips initial empty tokens and builds
 * the command step-by-step using helper functions.
 *
 * @param ta A pointer to the token array structure to be parsed.
 * @return Returns a pointer to the fully constructed `t_cmd` structure (or the head of a pipeline).
 * @return Returns NULL if parsing fails or if the resulting command is empty/invalid.
 */
t_cmd	*parse_tokens(t_ta *ta)
{
	t_cmd	*command_structure;
	int		token_idx;

	if (!ta || !ta->tokens)
		return (NULL);
	command_structure = cmd_init(); 
	if (!command_structure)
		return (NULL);
	token_idx = 0;
	while (token_idx < ta->t_tot && ft_strcmp(ta->tokens[token_idx], "") == 0)
		token_idx++;
	while (token_idx < ta->t_tot)
	{
		if (!process_token(command_structure, ta, &token_idx))
		{
			free_command(command_structure); 
			return (NULL);
		}
	}
	if (!command_structure->name && command_structure->arg_count == 0)
	{
		free_command(command_structure);
		return (NULL);
	}
	return (command_structure);
}
