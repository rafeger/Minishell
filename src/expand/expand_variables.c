#include "../../include/minishell.h"


char *get_var_name(t_expand_dollar *ed)
{
    int start = ed->pos[0];
    
    while (ft_isalnum(ed->input[ed->pos[0]]) || ed->input[ed->pos[0]] == '_')
        ed->pos[0]++;
    
    return (substr_range(ed->input, start, ed->pos[0]));
}


void expand_special(t_expand_dollar *ed)
{
    char *tmp;
    
    if (ed->input[ed->pos[0]] == '$')
        tmp = ft_itoa(getpid());
    else 
        tmp = ft_itoa(ed->shell->last_exit_status);
    
    if (tmp)
    {
        append_str(ed, tmp);
        free(tmp);
    }
    ed->pos[0]++;
}


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