#include "../../include/minishell.h"

// Main dollar expansion logic
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