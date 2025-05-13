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

