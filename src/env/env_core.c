#include "../../minishell.h"

/*
 * Creates formatted environment string "key=value".
 * Allocates and builds combined string.
 * Returns NULL if allocation fails.
 * Used when needing string format of env variable.
*/
char	*create_env_string(const char *key, const char *value)
{
	int		len;
	char	*env_string;

	len = ft_strlen(key) + ft_strlen(value) + 2;
	env_string = malloc(sizeof(char) * len);
	if (!env_string)
		return (NULL);
	ft_strcpy(env_string, key);
	ft_strcat(env_string, "=");
	ft_strcat(env_string, value);
	return (env_string);
}

/*
 * Searches for environemnt variable by key.
 * Returns pointer to value if found.
 * Returns NULL if key doesn't exist.
 * Used for environment variable lookup.
*/
void	*get_env_value(t_env *env, const char *key)
{
	while (env)
	{
		if (ft_strcmp(env->key, key) == 0)
			return (env->value);
		env = env->next;
	}
	return (NULL);
}

/*
 * Updates or adds environment variable.
 * If key exists, updates, value.
 * If key doesn't exist, creates new entry.
 * Handles memory management for value updates.
*/
void	set_env_value(t_env **env, const char *key, const char *value)
{
	t_env	*current;
	t_env	*new_env;

	current = *env;
	while (current)
	{
		if (ft_strcmp(current->key, key) == 0)
		{
			free(current->value);
			current->value = ft_strdup(value);
			return ;
		}
		current = current->next;
	}
	new_env = malloc(sizeof(t_env));
	if (!new_env)
		return ;
	new_env->key = ft_strdup(key);
	new_env->value = ft_strdup(value);
	new_env->next = *env;
	*env = new_env;
}

/*
 * Removes environment variable by key.
 * Handles list relinking after removal.
 * Frees removed node's memory.
 * Mainteains list integrity during removal.
*/
void	remove_env_var(t_env **env, const char *key)
{
	t_env	*current;
	t_env	*prev;

	current = *env;
	prev = NULL;
	while (current)
	{
		if (ft_strcmp(current->key, key) == 0)
		{
			if (prev)
				prev->next = current->next;
			else
				*env = current->next;
			free(current->key);
			free(current->value);
			free(current);
			return ;
		}
		prev = current;
		current = current->next;
	}
}

/*
 * Converts environment list to string array.
 * Creates array of "key=value" strings.
 * Used for execve environment preparation.
 * Returns NULL if any allocation fails.
*/
char	**env_list_to_array(t_env *env)
{
	int		count;
	char	**env_array;
	int		i;

	env_array = allocate_env_array(env, &count);
	if (!env_array)
		return (NULL);
	if (!fill_env_array(env_array, env))
	{
		i = 0;
		while (env_array[i])
			free(env_array[i++]);
		free(env_array);
		return (NULL);
	}
	return (env_array);
}
