#include "../../minishell.h"


static int	is_var_char(char c)
{
	return (ft_isalnum(c) || c == '_');
}

static char	*get_var_name(const char *str, int *i)
{
	int		start;
	int		len;
	char	*name;

	start = *i;
	len = 0;
	while (str[*i] && is_var_char(str[*i]))
	{
		len++;
		(*i)++;
	}
	name = malloc(len + 1);
	if (!name)
		return (NULL);
	ft_strncpy(name, str + start, len);
	name[len] = '\0';
	return (name);
}

static char	*get_var_value(char *name)
{
	char	*val;

	if (!name)
		return (NULL);
	val = getenv(name);
	if (!val)
		return (ft_strdup(""));
	return (ft_strdup(val));
}

static char	*append_and_free(char *base, char *to_append)
{
	char	*new;

	new = ft_strjoin(base, to_append);
	free(base);
	free(to_append);
	return (new);
}

static char	*expand_loop(const char *str)
{
	char	*res;
	char	*name;
	char	*val;
	int		i;
	int		start;

	i = 0;
	res = ft_strdup("");
	while (str[i])
	{
		if (str[i] == '$' && str[i + 1] && str[i + 1] != ' ' && str[i + 1] != '$')
		{
			i++;
			name = get_var_name(str, &i);
			val = get_var_value(name);
			free(name);
			if (!val)
				return (free(res), NULL);
			res = append_and_free(res, val);
		}
		else
		{
			start = i;
			while (str[i] && str[i] != '$')
				i++;
			val = ft_substr(str, start, i - start);
			if (!val)
				return (free(res), NULL);
			res = append_and_free(res, val);
		}
	}
	return (res);
}

char	*expand_variables(const char *str)
{
	if (!str)
		return (NULL);
	return (expand_loop(str));
}