#include "../../include/minishell.h"

int	check_dollar_concatenation(char *prev, char *curr)
{
    if (prev[0] == '$' && (curr[0] == ':' || curr[0] == '$' || curr[0] == '='))
        return (1);
    return (0);
}

int	check_colon_concatenation(char *prev, char *curr)
{
    int	prev_len;

    prev_len = ft_strlen(prev);
    if (prev_len > 0 && prev[prev_len - 1] == ':' && curr[0] == '$')
        return (1);
    return (0);
}

int	check_trailing_dollar(char *prev, char *curr)
{
    int	prev_len;

    prev_len = ft_strlen(prev);
    if (prev_len > 0 && prev[prev_len - 1] == '$')
    {
        if (curr[0] == '=')
            return (1);
        return (1);
    }
    return (0);
}

int	check_digit_concatenation(char *prev, char *curr)
{
    if (ft_isdigit(prev[0]) && curr[0] == '$')
        return (1);
    return (0);
}

int	should_merge_arguments(char *prev_arg, char *curr_arg)
{
    if (!prev_arg || !curr_arg || !*prev_arg || !*curr_arg)
        return (0);
    if (check_dollar_concatenation(prev_arg, curr_arg))
        return (1);
    if (check_colon_concatenation(prev_arg, curr_arg))
        return (1);
    if (check_trailing_dollar(prev_arg, curr_arg))
        return (1);
    if (check_digit_concatenation(prev_arg, curr_arg))
        return (1);
    return (0);
}