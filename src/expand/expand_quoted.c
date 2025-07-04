#include "../../include/minishell.h"

int	handle_quoted_var(char **result, int *j, char *input)
{
    int		quote_len;
    int		is_quoted;

    if (!input[1])
    {
        (*result)[(*j)++] = *input;
        return (0);
    }
    if (input[1] == '\'')
        return (0);
    is_quoted = 0;
    quote_len = get_quoted_length(input + 1);
    if (input > (char *)1)
        is_quoted = is_in_quotes(input);
    if (is_quoted)
    {
        (*result)[(*j)++] = *input;
        return (0);
    }
    return (handle_quoted_len(result, j, input, quote_len));
}
