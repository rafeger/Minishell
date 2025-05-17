#include "../../minishell.h"

char **append_arg(char **args, char *arg)
{
    int count = 0;
    while (args && args[count])
        count++;
    char **new_args = malloc(sizeof(char *) * (count + 2));
    if (!new_args)
        return NULL;
    int i = 0;
    while (i < count)
    {
        new_args[i] = args[i];
        i++;
    }
    new_args[count] = ft_strdup(arg);
    if (!new_args[count])
    {
        free(new_args);
        return NULL;
    }
    new_args[count + 1] = NULL;
    free(args);
    return (new_args);
}

t_command *parse_tokens_to_command(t_token *tok)
{
    t_command *head = NULL;
    t_command *cur = NULL;
    while (tok)
    {
        t_command *cmd = calloca(1, sizeof(t_command));
        if (!cmd)
        {
            free_command_list(head);
            return NULL;
        }
        cmd->infile = -1;
        cmd->outfile = -1;


    }
}