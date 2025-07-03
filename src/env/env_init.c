#include "../../include/minishell.h"

char	**alloc_env_strs(t_env *env_head, int *env_count)
{
    char	**strarr;
    t_env	*cursor;

    *env_count = 0;
    cursor = env_head;
    while (cursor)
    {
        (*env_count)++;
        cursor = cursor->next;
    }
    strarr = malloc(sizeof(char *) * (*env_count + 1));
    return (strarr);
}

int	populate_env_strs(char **strarr, t_env *env_head)
{
    int		idx;
    t_env	*cursor;

    idx = 0;
    cursor = env_head;
    while (cursor)
    {
        strarr[idx] = make_env_kv_string(cursor->key, cursor->value);
        if (!strarr[idx])
            return (0);
        idx++;
        cursor = cursor->next;
    }
    strarr[idx] = NULL;
    return (1);
}

t_env	*parse_env_pair(char *env_str)
{
    t_env	*entry;
    char	*eq_ptr;

    entry = malloc(sizeof(t_env));
    if (!entry)
        return (NULL);
    eq_ptr = ft_strchr(env_str, '=');
    if (!eq_ptr)
    {
        free(entry);
        return (NULL);
    }
    entry->key = ft_strndup(env_str, eq_ptr - env_str);
    entry->value = ft_strdup(eq_ptr + 1);
    entry->next = NULL;
    return (entry);
}

void	append_env_entry(t_env **env_head, t_env *new_entry)
{
    t_env	*cursor;

    if (!*env_head)
    {
        *env_head = new_entry;
        return ;
    }
    cursor = *env_head;
    while (cursor->next)
        cursor = cursor->next;
    cursor->next = new_entry;
}

t_env	*build_env_list(char **envp)
{
    t_env	*env_head;
    t_env	*entry;
    int		idx;

    env_head = NULL;
    idx = 0;
    while (envp[idx])
    {
        entry = parse_env_pair(envp[idx]);
        if (entry)
            append_env_entry(&env_head, entry);
        idx++;
    }
    return (env_head);
}