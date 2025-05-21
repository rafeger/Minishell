#include "../../minishell.h"

bool ft_isspace(char c)
{
	if (c == ' ' || c == '\t')
		return (true);
	return (false);
}

void	handle_whitespaces(char *str, int *i)
{
	while (str[*i] == ' ' || str[*i] == '\t')
		(*i)++;
}

char	**list_to_str_array(t_list *lst)
{
	int		size = 0;
	t_list	*tmp = lst;
	char	**arr;
	int		i = 0;

	while (tmp)
	{
		size++;
		tmp = tmp->next;
	}
	arr = malloc(sizeof(char *) * (size + 1));
	if (!arr)
		return (NULL);
	tmp = lst;
	while (tmp)
	{
		arr[i] = ft_strdup(tmp->content);
		if (!arr[i])
		{
			while (i--)
				free(arr[i]);
			free(arr);
			return (NULL);
		}
		i++;
		tmp = tmp->next;
	}
	arr[i] = NULL;
	return (arr);
}

