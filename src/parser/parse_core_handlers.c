#include "../../include/minishell.h"

int	is_empty_string_token(t_ta *t_a, int i)
{
    return (ft_strcmp(t_a->tokens[i], "") == 0);
}

int	find_file_token_position(t_ta *t_a, int start_pos)
{
    int	file_pos;

    file_pos = start_pos + 1;
    while (file_pos < t_a->t_tot
        && ft_strcmp(t_a->tokens[file_pos], " ") == 0)
        file_pos++;
    return (file_pos);
}

void	process_redirection_token(t_cmd *cmd, t_ta *t_a, int *i)
{
    int	redir_type;
    int	file_pos;

    redir_type = get_redirect_type(t_a->tokens[*i]);
    if (redir_type >= 0)
    {
        file_pos = find_file_token_position(t_a, *i);
        if (file_pos < t_a->t_tot && t_a->tokens[file_pos])
        {
            cmd_add_redirect(cmd, redir_type, t_a->tokens[file_pos],
                t_a->quoted[file_pos]);
            *i = file_pos + 1;
        }
        else
            (*i)++;
    }
}

void	process_empty_string_token(t_cmd *cmd, t_ta *t_a, int *i)
{
    char	*space_str;

    if ((*i + 1) < t_a->t_tot && ft_strcmp(t_a->tokens[*i + 1], " ") == 0
        && !t_a->quoted[*i + 1])
    {
        space_str = ft_strdup(t_a->tokens[*i + 1]);
        if (space_str)
        {
            add_arg(cmd, space_str, t_a->quoted[*i + 1]);
            free(space_str);
        }
    }
    else
    {
        add_arg(cmd, "", t_a->quoted[*i]);
    }
}

int	handle_next_token(t_cmd *cmd, t_ta *t_a, int *i)
{
    if (!handle_pipe_token(cmd, t_a, i))
        return (0);
    if (*i >= t_a->t_tot)
        return (1);
    if (should_skip_space_token(cmd, t_a, *i))
    {
        (*i)++;
        return (1);
    }
    if (is_redirection_token(t_a, *i))
        process_redirection_token(cmd, t_a, i);
    else if (is_empty_string_token(t_a, *i))
    {
        process_empty_string_token(cmd, t_a, i);
        (*i)++;
    }
    else
    {
        add_arg(cmd, t_a->tokens[*i], t_a->quoted[*i]);
        (*i)++;
    }
    return (1);
}
