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
