#include "../../include/minishell.h"

static t_cmd	*setup_pipe_cmd(t_ta *new_ta, t_ta *t_array, int idx, char **stok)
{
	t_cmd	*next_cmd;

	new_ta->tokens = stok;
	if (!init_quoted_array(new_ta, t_array, idx))
	{
		cleanup_pipe_data(new_ta, stok, new_ta->t_tot);
		return (NULL);
	}
	next_cmd = parse_tokens(new_ta);
	free_tokenarray(new_ta);
	return (next_cmd);
}

int	handle_pipe(t_cmd *cmd, t_ta *t_array, int index)
{
	t_ta	*new_ta;
	t_cmd	*next_cmd;
	char	**sub_tokens;

	if (index + 1 >= t_array->t_tot)
		return (0);
	new_ta = init_new_ta(t_array, index);
	if (!new_ta)
		return (0);
	sub_tokens = create_sub_tokens(t_array, index, new_ta);
	if (!sub_tokens)
	{
		cleanup_pipe_data(new_ta, NULL, 0);
		return (0);
	}
	next_cmd = setup_pipe_cmd(new_ta, t_array, index, sub_tokens);
	if (!next_cmd)
		return (0);
	next_cmd->prev = cmd;
	cmd->next = next_cmd;
	return (1);
}

static int	handle_pipe_token(t_cmd *cmd, t_ta *t_array, int *i)
{
	if (ft_strcmp(t_array->tokens[*i], "|") == 0 && !t_array->quoted[*i])
	{
		if (!handle_pipe(cmd, t_array, *i))
		{
			free_command(cmd); 
			return (0);
		}
		*i = t_array->t_tot; 
	}
	return (1);
}

static void	handle_redirect(t_cmd *cmd, t_ta *t_array, int *i)
{
	int	redir_kind;
	int	file_token_idx;

	redir_kind = get_redirect_type(t_array->tokens[*i]);
	if (redir_kind >= 0)
	{
		file_token_idx = *i + 1;
		while (file_token_idx < t_array->t_tot && ft_strcmp(t_array->tokens[file_token_idx], " ") == 0)
			file_token_idx++;
		if (file_token_idx < t_array->t_tot && t_array->tokens[file_token_idx])
		{
			add_redirect(cmd, redir_kind, t_array->tokens[file_token_idx], t_array->quoted[file_token_idx]);
			*i = file_token_idx + 1; 
		}
		else
			(*i)++;
	}
}


static void	handle_empty_token(t_cmd *cmd, t_ta *t_array, int *i)
{
	char	*space_content;

    if ((*i + 1) < t_array->t_tot && ft_strcmp(t_array->tokens[*i + 1], " ") == 0 && !t_array->quoted[*i + 1])
    {
        space_content = ft_strdup(t_array->tokens[*i + 1]);
        if (space_content)
        {
            add_argument(cmd, space_content, t_array->quoted[*i + 1]); 
            free(space_content);
        }
    }
    else
    {
        add_argument(cmd, "", t_array->quoted[*i]);
	}
}


static int	process_token(t_cmd *cmd, t_ta *t_array, int *i)
{
	if (!handle_pipe_token(cmd, t_array, i))
		return (0);
	if (*i >= t_array->t_tot)
		return (1);
	if (ft_strcmp(t_array->tokens[*i], " ") == 0 && !t_array->quoted[*i] && !cmd->c_name)
	{
		(*i)++; 
		return (1);
	}
	if (get_redirect_type(t_array->tokens[*i]) >= 0 && !t_array->quoted[*i])
	{
		handle_redirect(cmd, t_array, i);
	}
	else if (ft_strcmp(t_array->tokens[*i], "") == 0)
	{
		handle_empty_token(cmd, t_array, i);
        (*i)++; 
	}
	else
	{
		add_argument(cmd, t_array->tokens[*i], t_array->quoted[*i]);
		(*i)++; 
	}
	return (1);
}

t_cmd	*parse_tokens(t_ta *t_array)
{
	t_cmd	*command_structure;
	int		token_pos;

	if (!t_array || !t_array->tokens)
		return (NULL);
	command_structure = cmd_initialisation(); 
	if (!command_structure)
		return (NULL);
	token_pos = 0;
	while (token_pos < t_array->t_tot && ft_strcmp(t_array->tokens[token_pos], "") == 0)
		token_pos++;
	while (token_pos < t_array->t_tot)
	{
		if (!process_token(command_structure, t_array, &token_pos))
		{
			free_command(command_structure); 
			return (NULL);
		}
	}
	if (!command_structure->c_name && command_structure->arg_count == 0
		&& command_structure->redirects == NULL)
	{
		free_command(command_structure);
		return (NULL);
	}
	return (command_structure);
}
