
#include "../../include/minishell.h"

/*
 * Sets up command structure for pipe segment.
 * Creates new token array and command structure.
 * Manages quote status and token copying.
 * Returns initialized command structure or NULL on failure.
*/
t_cmd	*setup_pipe_cmd(t_ta *new_ta, t_ta *ta, int idx, char **stok)
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

/*
 * Creates new pipe for command.
 * Initializes fd_info if needed.
 * Returns 1 on pipe creation failure, 0 on success.
*/
int	create_pipe_for_cmd(t_cmd *cmd)
{
	if (!cmd->fd_info)
		fd_info_init(cmd);
	if (pipe(cmd->fd_info->pipe_fd) == -1)
		return (-1);
	return (0);
}

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

/*
 * Creates pipes for all commands in pipeline.
 * Initializes file descriptor information.
 * Essential for setting up pipeline communication.
*/
void	create_all_pipes(t_cmd *cmd_list)
{
	t_cmd	*cmd;

	cmd = cmd_list;
	while (cmd)
	{
		fd_info_init(cmd);
		if (cmd->next)
			create_pipe_for_cmd(cmd);
		cmd = cmd->next;
	}
}
