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
	if (!command_structure->name && command_structure->arg_count == 0
		&& command_structure->redirects == NULL)
	{
		free_command(command_structure);
		return (NULL);
	}
	return (command_structure);
}
