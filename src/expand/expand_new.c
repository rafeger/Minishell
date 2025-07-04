#include "../../include/minishell.h"

// typedef struct s_expand_dollar
// {
//     int     pos[4];         // [0]=input_pos, [1]=output_pos, [2]=single_quote, [3]=double_quote
//     int     total_size;
//     char    *result;
//     t_env   *env;
//     t_shell_data *shell;
//     char    *input;
// } t_expand_dollar;

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

// Variable name extraction
char *get_var_name(t_expand_dollar *ed)
{
    int start = ed->pos[0];
    
    while (ft_isalnum(ed->input[ed->pos[0]]) || ed->input[ed->pos[0]] == '_')
        ed->pos[0]++;
    
    return (substr_range(ed->input, start, ed->pos[0]));
}

// Helper function to append and update
void append_str(t_expand_dollar *ed, char *src)
{
    ft_strcat(ed->result, src);
    ed->pos[1] += ft_strlen(src);
}

// Handle special variables ($?, $$)
void expand_special(t_expand_dollar *ed)
{
    char *tmp;
    
    if (ed->input[ed->pos[0]] == '$')
        tmp = ft_itoa(getpid());
    else // $?
        tmp = ft_itoa(ed->shell->last_exit_status);
    
    if (tmp)
    {
        append_str(ed, tmp);
        free(tmp);
    }
    ed->pos[0]++;
}

// Handle regular variables ($VAR)
void expand_var(t_expand_dollar *ed)
{
    char *name = get_var_name(ed);
    if (name)
    {
        char *value = find_env_val(ed->env, name);
        if (value)
            append_str(ed, value);
        free(name);
    }
}
// Check if we have empty quotes after $
int is_empty_quotes(t_expand_dollar *ed)
{
    if (ed->input[ed->pos[0]] == '"' && ed->input[ed->pos[0] + 1] == '"')
        return (2); // Return 2 chars to skip
    if (ed->input[ed->pos[0]] == '\'' && ed->input[ed->pos[0] + 1] == '\'')
        return (2); // Return 2 chars to skip
    return (0);
}

// Main dollar expansion logic (FIXED)
void expand_dollar(t_expand_dollar *ed)
{
    int empty_quote_len;
    
    ed->pos[0]++; // Skip the '$'
    
    // Check for empty quotes first
    empty_quote_len = is_empty_quotes(ed);
    if (empty_quote_len > 0)
    {
        // $"" or $'' expands to nothing, just skip the quotes
        ed->pos[0] += empty_quote_len;
        return;
    }
    
    if (ft_isalnum(ed->input[ed->pos[0]]) || ed->input[ed->pos[0]] == '_')
        expand_var(ed);
    else if (ed->input[ed->pos[0]] == '$' || ed->input[ed->pos[0]] == '?')
        expand_special(ed);
    else
    {
        // Literal '$'
        ed->result[ed->pos[1]++] = '$';
        ed->result[ed->pos[1]] = '\0';
    }
}

// Size calculation for special variables
int calc_special_size(t_expand_dollar *ed)
{
    int     size;
    char    *tmp;

    size = 0;
    if (ed->input[ed->pos[0]] == '$')
        tmp = ft_itoa(getpid());
    else
        tmp = ft_itoa(ed->shell->last_exit_status);
    if (tmp)
    {
        size = ft_strlen(tmp);
        free(tmp);
    }
    ed->pos[0]++;
    return (size);
}

// Size calculation for dollar expansion
int calc_dollar_size(t_expand_dollar *ed)
{
    int     size;
    char    *name;
    char    *value;
    int     empty_quote_len;

    size = 0;
    ed->pos[0]++;
    
    empty_quote_len = is_empty_quotes(ed);
    if (empty_quote_len > 0)
    {
        ed->pos[0] += empty_quote_len;
        return (0);
    }
    if (ft_isalnum(ed->input[ed->pos[0]]) || ed->input[ed->pos[0]] == '_')
    {
        name = get_var_name(ed);
        if (name)
        {
            value = find_env_val(ed->env, name);
            if (value)
                size = ft_strlen(value);
            free(name);
        }
    }
    else if (ed->input[ed->pos[0]] == '$' || ed->input[ed->pos[0]] == '?')
        size = calc_special_size(ed);
    else
        size = 1;
    return (size);
}

// Total size calculation
int calc_total_size(t_expand_dollar *ed)
{
    int size = 0;
    
    if (ed->input == NULL || ed->env == NULL)
        return (0);
    ed->pos[0] = 0;
    ed->pos[2] = -1;  // Outside single quotes
    ed->pos[3] = -1;  // Outside double quotes
    
    while (ed->input[ed->pos[0]])
    {
        update_quotes(ed);
        if (ed->input[ed->pos[0]] == '$' && ed->pos[2] == -1)
            size += calc_dollar_size(ed);
        else
        {
            size++;
            ed->pos[0]++;
        }
    }
    return (size);
}

// Process string expansion
void process_expansion(t_expand_dollar *ed)
{
    while (ed->input[ed->pos[0]])
    {
        update_quotes(ed);
        if (ed->input[ed->pos[0]] == '$' && ed->pos[2] == -1)
            expand_dollar(ed);
        else
        {
            ed->result[ed->pos[1]++] = ed->input[ed->pos[0]++];
            ed->result[ed->pos[1]] = '\0';
        }
    }
}

// Main expansion function
char *expand_variables(char *input, t_shell_data *shell_data)
{
    t_expand_dollar ed;
    
    if (!input)
        return (NULL);
    
    // Initialize
    ed.env = shell_data->env;
    ed.input = input;
    ed.shell = shell_data;
    ed.pos[0] = 0;
    ed.pos[1] = 0;
    ed.pos[2] = -1;  // Outside single quotes
    ed.pos[3] = -1;  // Outside double quotes

    ed.total_size = calc_total_size(&ed);
    ed.pos[0] = 0;
    ed.result = malloc(sizeof(char) * (ed.total_size + 1));
    if (!ed.result)
        return (NULL);
    ed.result[0] = '\0';

    process_expansion(&ed);
    ed.result[ed.pos[1]] = '\0';
    
    return (ed.result);
}