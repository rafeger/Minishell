#include "../../include/minishell.h"

int	get_var_length(const char *str)
{
	int	i;

	i = 0;
	if (str[i] == '?')
		return (1);
	if (ft_isdigit(str[i]))
		return (1);
	while (str[i] && (ft_isalnum(str[i]) || str[i] == '_'))
		i++;
	return (i);
}

char	*get_var_value(const char *var, t_shell_data *shell_data)
{
	static char		*exit_status;

	exit_status = NULL;
	if (ft_strcmp(var, "?") == 0)
	{
		if (exit_status)
			free(exit_status);
		exit_status = ft_itoa(shell_data->last_exit_status);
		if (!exit_status)
			return (NULL);
		return (exit_status);
	}
	return (find_env_val(shell_data->env, var));
}

void	process_expand_char(t_exp *exp, char *input)
{
	if (input[exp->i] == '\'' && !exp->dquote)
		exp->squote = !exp->squote;
	else if (input[exp->i] == '"' && !exp->squote)
		exp->dquote = !exp->dquote;
	if (input[exp->i] == '$' && !exp->squote && input[exp->i + 1])
		exp->i += handle_var(&exp->result, &exp->j, input + exp->i, exp->shell);
	else
		exp->result[exp->j++] = input[exp->i];
}

size_t	calculate_expanded_size(char *input, t_shell_data *shell_data)
{
	size_t	size;
	int		i;
	int		in_dquote;
	int		in_squote;

	size = 0;
	i = 0;
	in_dquote = 0;
	in_squote = 0;
	while (input[i])
	{
		if (input[i] == '"' && !in_squote)
			in_dquote = !in_dquote;
		if (input[i] == '\'' && !in_dquote)
			in_squote = !in_squote;
		if (input[i] == '$' && !in_squote && input[i + 1])
			i += process_var(input, i, &size, shell_data);
		else
			size++;
		i++;
	}
	return (size + 1);
}

char	*expand_variables(char *input, t_shell_data *shell_data)
{
	t_exp	exp;
	size_t	size;

	size = calculate_expanded_size(input, shell_data);
	exp.result = calloc(size + 1, sizeof(char));
	if (!exp.result)
		return (NULL);
	exp.i = 0;
	exp.j = 0;
	exp.squote = 0;
	exp.dquote = 0;
	exp.shell = shell_data;
	while (input[exp.i])
	{
		process_expand_char(&exp, input);
		exp.i++;
	}
	exp.result[exp.j] = '\0';
	return (exp.result);
}

int	handle_quoted_len(char **result, int *j, char *input, int quote_len)
{
	if (quote_len == 1 && input[2] == input[1])
		return (quote_len);
	if (quote_len == 1 && input[3])
	{
		ft_memcpy(*result + *j, input + 3, ft_strlen(input + 3));
		*j += ft_strlen(input + 3);
		return (quote_len);
	}
	if (quote_len)
	{
		ft_memcpy(*result + *j, input + 2, quote_len - 1);
		*j += quote_len - 1;
		return (quote_len);
	}
	(*result)[(*j)++] = *input;
	return (0);
}


int	process_var(char *input, int i, size_t *size, t_shell_data *sd)
{
	int		var_len;
	char	var_name[256];
	char	*var_value;

	var_len = get_var_length(input + i + 1);
	if (var_len > 0)
	{
		ft_strncpy(var_name, input + i + 1, var_len);
		var_name[var_len] = '\0';
		var_value = get_var_value(var_name, sd);
		if (var_value)
			*size += ft_strlen(var_value) + 2;
		else
			(*size)++;
		if (var_value && ft_strcmp(var_name, "?") == 0)
			free(var_value);
		return (var_len);
	}
	(*size)++;
	return (0);
}



/*
 * Check if string starts with quotes.
 * Returns 1 if yes, 0 otherwise.
 * Used for quote-aware expansion.
*/
int	is_in_quotes(const char *str)
{
	if ((str[0] == '"' || str[0] == '\'') && str[1] != '\0')
		return (1);
	return (0);
}

/*
 * Copies variable value to result string.
 * Handles proper quoting of value.
 * Updates position in result string.
 * Used during variable expansion process.
*/
void	copy_var_value(char **result, int *j, char *var_value)
{
	(*result)[*j] = '"';
	(*j)++;
	ft_strcpy(*result + *j, var_value);
	*j += ft_strlen(var_value);
	(*result)[*j] = '"';
	(*j)++;
}

/*
 * Gets length of quoted string.
 * Finds matching closing quote.
 * Returns length including quotes.
 * Used for quote-aware string processing.
*/
int	get_quoted_length(const char *str)
{
	int		i;
	char	quote_char;

	i = 1;
	quote_char = str[0];
	if (!str[i])
		return (0);
	while (str[i] && str[i] != quote_char)
		i++;
	if (str[i] == quote_char)
		return (i);
	return (0);
}

/*
 * Handles expansion of quoted variables.
 * Manages quote removal and content copying.
 * Returns number of characters processed.
 * Used for variable expansion within quotes.
*/
int	handle_quoted_var(char **result, int *j, char *input)
{
	int		quote_len;
	int		is_quoted;

	if (!input[1])
	{
		(*result)[(*j)++] = *input;
		return (0);
	}
	if (input[1] == '\'')
		return (0);
	is_quoted = 0;
	quote_len = get_quoted_length(input + 1);
	if (input > (char *)1)
		is_quoted = is_in_quotes(input);
	if (is_quoted)
	{
		(*result)[(*j)++] = *input;
		return (0);
	}
	return (handle_quoted_len(result, j, input, quote_len));
}

/*
 * Processes variable during expansion.
 * Handles variable lookup and substitution.
 * Returns number of characters processed.
 * Core function for variable expansion.
*/
int	handle_var(char **res, int *j, char *in, t_shell_data *sd)
{
    if (in[1] == '"' && in[2] == '"')
        return 2;
    if (in[1] == '"')
    {
        int k = 2;
        while (in[k] && in[k] != '"')
            (*res)[(*j)++] = in[k++];
        if (in[k] == '"')
            return k;
        (*res)[(*j)++] = *in;
        return 0;
    }
    if (in[1] == '\'' && in[2] == '\'')
        return 2;
    if (in[1] == '\'')
    {
        int k = 2;
        while (in[k] && in[k] != '\'')
            (*res)[(*j)++] = in[k++];
        if (in[k] == '\'')
            return k;
        (*res)[(*j)++] = *in;
        return 0;
    }
	int		var_len;
	char	*var_value;
	char	var_name[256];

	if (is_in_quotes(in + 1))
		return (handle_quoted_var(res, j, in));
	var_len = get_var_length(in + 1);
	if (var_len > 0)
	{
		ft_strncpy(var_name, in + 1, var_len);
		var_name[var_len] = '\0';
		var_value = get_var_value(var_name, sd);
		if (var_value)
		{
			copy_var_value(res, j, var_value);
			if (ft_strcmp(var_name, "?") == 0)
				free(var_value);
		}
		return (var_len);
	}
	(*res)[(*j)++] = *in;
	return (0);
}
