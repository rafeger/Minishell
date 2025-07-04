#include "../../include/minishell.h"

// Helper function to create substring
char *substr_range(char *str, int start, int end)
{
    char    *new;
    int     i;
    size_t  len;

    if (str == NULL)
        return (NULL);
    len = ft_strlen(str);
    if (start < 0 || end < 0 || start > end
        || start > (int)len || end > (int)len)
        return (NULL);
    new = malloc(sizeof(char) * ((end - start) + 1));
    if (new == NULL)
        return (NULL);
    i = 0;
    while (start < end)
    {
        new[i] = str[start];
        i++;
        start++;
    }
    new[i] = '\0';
    return (new);
}

// Quote handling
void update_quotes(t_expand_dollar *ed)
{
    if (ed->input[ed->pos[0]] == '\'' && ed->pos[3] == -1)
        ed->pos[2] *= -1;
    else if (ed->input[ed->pos[0]] == '"' && ed->pos[2] == -1)
        ed->pos[3] *= -1;
}

// Helper function to append and update
void append_str(t_expand_dollar *ed, char *src)
{
    ft_strcat(ed->result, src);
    ed->pos[1] += ft_strlen(src);
}

// Check if we have empty quotes after $ for special $"" or $'' case
int is_empty_quotes(t_expand_dollar *ed)
{
    if (ed->input[ed->pos[0]] == '"' && ed->input[ed->pos[0] + 1] == '"')
        return (2);
    if (ed->input[ed->pos[0]] == '\'' && ed->input[ed->pos[0] + 1] == '\'')
        return (2);
    return (0);
}