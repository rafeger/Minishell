#include "../../minishell.h"

static int	is_special_c(char c)
{
	return (c == '|' || c == '<' || c == '>');
}

static int	token_len(const char *str, int *quote_flag)
{
	int		i;
	char	q;

	i = 0;
	q = 0;
	while (str[i])
	{
		if (!q && (str[i] == '\'' || str[i] == '"'))
			q = str[i];
		else if (q && str[i] == q)
			q = 0;
		else if (!q && (ft_isspace(str[i]) || is_special_c(str[i])))
			break ;
		i++;
	}
	if (quote_flag)
		*quote_flag = (q != 0);
	return (i);
}

static char	*dup_token_part(const char *start, int len)
{
	char	*res;

	res = malloc(len + 1);
	if (!res)
		return (NULL);
	ft_strncpy(res, start, len);
	res[len] = '\0';
	return (res);
}

static char	*join_and_free(char *s1, char *s2)
{
	char	*joined;

	joined = ft_strjoin(s1, s2);
	free(s1);
	free(s2);
	return (joined);
}

static char	*parse_word(const char *str, int *i)
{
	char	*result;
	int		quote_open;
	int		part_len;
	char	*part;

	result = NULL;
	quote_open = 0;
	while (str[*i] && !ft_isspace(str[*i]) && !is_special_c(str[*i]))
	{
		part_len = token_len(&str[*i], &quote_open);
		if (quote_open)
		{
			print_syntax_error("quotes not closed !\n");
			return (free(result), NULL);
		}
		part = dup_token_part(&str[*i], part_len);
		if (!part)
			return (free(result), NULL);
		result = join_and_free(result, part);
		*i += part_len;
	}
	return (result);
}

static char	*parse_operator(const char *str, int *i)
{
	if ((str[*i] == '<' && str[*i + 1] == '<') ||
		(str[*i] == '>' && str[*i + 1] == '>'))
	{
		char	*op;

		op = dup_token_part(&str[*i], 2);
		*i += 2;
		return (op);
	}
	else
	{
		char	*op;

		op = dup_token_part(&str[*i], 1);
		(*i)++;
		return (op);
	}
}

char	*extract_token(const char *str, int *i)
{
	while (ft_isspace(str[*i]))
		(*i)++;
	if (!str[*i])
		return (NULL);
	if (is_special_c(str[*i]))
		return (parse_operator(str, i));
	else
		return (parse_word(str, i));
}
