#include "../../include/minishell.h"


void expand_dollar(t_expand_dollar *ed)
{
    int empty_quote_len;
    
    ed->pos[0]++; 
    
    
    empty_quote_len = is_empty_quotes(ed);
    if (empty_quote_len > 0)
    {
        
        ed->pos[0] += empty_quote_len;
        return;
    }
    
    if (ft_isalnum(ed->input[ed->pos[0]]) || ed->input[ed->pos[0]] == '_')
        expand_var(ed);
    else if (ed->input[ed->pos[0]] == '$' || ed->input[ed->pos[0]] == '?')
        expand_special(ed);
    else
    {
        
        ed->result[ed->pos[1]++] = '$';
        ed->result[ed->pos[1]] = '\0';
    }
}