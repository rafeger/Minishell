#include "../../include/minishell.h"

int	skip_leading_empty_tokens(t_ta *t_a)
{
    int	pos;

    pos = 0;
    while (pos < t_a->t_tot && ft_strcmp(t_a->tokens[pos], "") == 0)
        pos++;
    return (pos);
}

t_cmd	*finalize_command_or_cleanup(t_cmd *cmd)
{
    if (!cmd->c_name && cmd->arg_count == 0 && cmd->redirects == NULL)
    {
        free_command(cmd);
        return (NULL);
    }
    return (cmd);
}

int	process_all_tokens(t_cmd *cmd, t_ta *t_a)
{
    int	token_pos;

    token_pos = skip_leading_empty_tokens(t_a);
    while (token_pos < t_a->t_tot)
    {
        if (!handle_next_token(cmd, t_a, &token_pos))
            return (0);
    }
    return (1);
}

int	should_skip_space_token(t_cmd *cmd, t_ta *t_a, int i)
{
    return (ft_strcmp(t_a->tokens[i], " ") == 0
        && !t_a->quoted[i] && !cmd->c_name);
}

int	is_redirection_token(t_ta *t_a, int i)
{
    return (get_redirect_type(t_a->tokens[i]) >= 0 && !t_a->quoted[i]);
}