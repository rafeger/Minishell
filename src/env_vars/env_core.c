#include "../../include/minishell.h"

char	*key_value_string(const char *k, const char *v)
{
	int		len;
	char	*kv_str;

	len = ft_strlen(k) + ft_strlen(v) + 2;
	kv_str = malloc(sizeof(char) * len);
	if (!kv_str)
		return (NULL);
	ft_strcpy(kv_str, k);
	ft_strcat(kv_str, "=");
	ft_strcat(kv_str, v);
	return (kv_str);
}

char	*find_env_val(t_env *env_head, const char *k)
{
	while (env_head)
	{
		if (ft_strcmp(env_head->key, k) == 0)
			return (env_head->value);
		env_head = env_head->next;
	}
	return (NULL);
}

void	set_env_variable(t_env **env_head, const char *k, const char *v)
{
	t_env	*cursor;
	t_env	*new_entry;

	cursor = *env_head;
	while (cursor)
	{
		if (ft_strcmp(cursor->key, k) == 0)
		{
			free(cursor->value);
			cursor->value = ft_strdup(v);
			return ;
		}
		cursor = cursor->next;
	}
	new_entry = malloc(sizeof(t_env));
	if (!new_entry)
		return ;
	new_entry->key = ft_strdup(k);
	new_entry->value = ft_strdup(v);
	new_entry->next = *env_head;
	*env_head = new_entry;
}

void	delete_env_entry(t_env **env_head, const char *k)
{
	t_env	*cursor;
	t_env	*last;

	cursor = *env_head;
	last = NULL;
	while (cursor)
	{
		if (ft_strcmp(cursor->key, k) == 0)
		{
			if (last)
				last->next = cursor->next;
			else
				*env_head = cursor->next;
			free(cursor->key);
			free(cursor->value);
			free(cursor);
			return ;
		}
		last = cursor;
		cursor = cursor->next;
	}
}

char	**envlist_to_strarr(t_env *env_head)
{
	int		env_count;
	char	**strarr;
	int		idx;

	strarr = env_alloc(env_head, &env_count);
	if (!strarr)
		return (NULL);
	if (!fill_env_string(strarr, env_head))
	{
		idx = 0;
		while (strarr[idx])
			free(strarr[idx++]);
		free(strarr);
		return (NULL);
	}
	return (strarr);
}