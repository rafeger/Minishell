#include "../../include/minishell.h"


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


char *expand_variables(char *input, t_shell_data *shell_data)
{
    t_expand_dollar ed;
    
    if (!input)
        return (NULL);
    ed.env = shell_data->env;
    ed.input = input;
    ed.shell = shell_data;
    ed.pos[0] = 0;
    ed.pos[1] = 0;
    ed.pos[2] = -1;
    ed.pos[3] = -1;

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