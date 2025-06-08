#include "../../minishell.h"

/*
 * Allocates memory for environment variable array.
 * Counts total environment variables in linked list.
 * Returns allocated array or NULL on failure.
 * Used when converting env list to array format.
*/
char	**allocate_env_array(t_env *env, int *count)
{
	char	**env_array;
	t_env	*current;

	*count = 0;
	current = env;
	while (current)
	{
		(*count)++;
		current = current->next;
	}
	env_array = malloc(sizeof(char *) * (*count + 1));
	return (env_array);
}

/*
 * Fills envrionment array with formatted strings.
 * Converts each env node to "key=value" format.
 * Returns 1 on success, 0 on any allocation failure.
 * Used to prepare env for external command execution.
*/
int	fill_env_array(char **env_array, t_env *env)
{
	int		i;
	t_env	*current;

	i = 0;
	current = env;
	while (current)
	{
		env_array[i] = create_env_string(current->key, current->value);
		if (!env_array[i])
			return (0);
		i++;
		current = current->next;
	}
	env_array[i] = NULL;
	return (1);
}

/*
 * Creates new environment variable node from string.
 * Parses input format "key=value".
 * Allocates and initializes node structure.
 * Returns NULL if string format invalid or allocation fails.
*/
t_env	*create_env_node(char *env_str)
{
	t_env	*node;
	char	*equals_sign;

	node = malloc(sizeof(t_env));
	if (!node)
		return (NULL);
	equals_sign = ft_strchr(env_str, '=');
	if (!equals_sign)
	{
		free(node);
		return (NULL);
	}
	node->key = ft_strndup(env_str, equals_sign - env_str);
	node->value = ft_strdup(equals_sign + 1);
	node->next = NULL;
	return (node);
}

/*
 * Adds new environment node to list.
 * If list is empty, sets as first node.
 * Otherwise, adds to end of list.
 * Maintains proper linking of environment variables.
*/
void	add_env_node(t_env **env_list, t_env *new_node)
{
	t_env	*current;

	if (!*env_list)
	{
		*env_list = new_node;
		return ;
	}
	current = *env_list;
	while (current->next)
		current = current->next;
	current->next = new_node;
}

/*
 * Initializes complete environment variable list.
 * Processes each string from envp array.
 * Creates and links all environment nodes.
 * Returns head of created environment list.
*/
t_env	*init_env(char **envp)
{
	t_env	*env_list;
	t_env	*env_node;
	int		i;

	env_list = NULL;
	i = 0;
	while (envp[i])
	{
		env_node = create_env_node(envp[i]);
		if (env_node)
			add_env_node(&env_list, env_node);
		i++;
	}
	return (env_list);
}
