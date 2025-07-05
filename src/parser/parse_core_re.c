#include "../../include/minishell.h"

int	validate_token_array(t_ta *t_a)
{
    return (t_a && t_a->tokens);
}

t_cmd	*initialize_command_structure(void)
{
    return (cmd_initialisation());
}

t_cmd	*cleanup_and_return_null(t_cmd *cmd)
{
    free_command(cmd);
    return (NULL);
}

int	is_empty_command(t_cmd *cmd)
{
    return (!cmd->c_name && cmd->arg_count == 0 && cmd->redirects == NULL);
}

t_cmd	*parse_tokens(t_ta *t_a)
{
    t_cmd	*cmd;

    if (!validate_token_array(t_a))
        return (NULL);
    cmd = initialize_command_structure();
    if (!cmd)
        return (NULL);
    if (!process_all_tokens(cmd, t_a))
        return (cleanup_and_return_null(cmd));
    return (finalize_command_or_cleanup(cmd));
}