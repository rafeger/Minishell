#include "../../include/minishell.h"

static int	handle_quote_case(char **res, int *j, char *in, char quote)
{
    int	k;

    k = 2;
    while (in[k] && in[k] != quote)
        (*res)[(*j)++] = in[k++];
    if (in[k] == quote)
        return (k);
    (*res)[(*j)++] = *in;
    return (0);
}

static int	handle_var_value(char **res, int *j, char *in, t_shell_data *sd)
{
    int		var_len;
    char	var_name[256];
    char	*var_value;

    var_len = get_var_length(in + 1);
    if (var_len > 0)
    {
        ft_strncpy(var_name, in + 1, var_len);
        var_name[var_len] = '\0';
        var_value = get_var_value(var_name, sd);
        if (var_value)
        {
            copy_var_value(res, j, var_value);
            if (ft_strcmp(var_name, "?") == 0)
                free(var_value);
        }
        return (var_len);
    }
    (*res)[(*j)++] = *in;
    return (0);
}

int	handle_var(char **res, int *j, char *in, t_shell_data *sd)
{
    if ((in[1] == '"' && in[2] == '"') || (in[1] == '\'' && in[2] == '\''))
        return (2);
    if (in[1] == '"')
        return (handle_quote_case(res, j, in, '"'));
    if (in[1] == '\'')
        return (handle_quote_case(res, j, in, '\''));
    if (is_in_quotes(in + 1))
        return (handle_quoted_var(res, j, in));
    return (handle_var_value(res, j, in, sd));
}