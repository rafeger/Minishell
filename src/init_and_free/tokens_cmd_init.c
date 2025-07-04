#include "../../include/minishell.h"

static int	*alloc_quoted(int cap)
{
    int	*quoted;

    quoted = malloc(sizeof(int) * cap);
    if (!quoted)
        return (NULL);
    ft_memset(quoted, 0, sizeof(int) * cap);
    return (quoted);
}

void	tokenarray_init_second(t_ta *t_array)
{	
    if (!t_array)
        return;
    t_array->tokenindex = 0;
    t_array->in_q = 0;
    t_array->quotechar = '\0';
    t_array->token = NULL;
    t_array->tokensize = 0;
    t_array->trailing_space = 0;
    t_array->second_quote = 0;
    t_array->t_tot = 0;
    t_array->cap = 10;
}

t_ta	*tokenarray_init(void)
{
    t_ta	*t_array;

    t_array = malloc(sizeof(t_ta));
    if (!t_array)
        return (NULL);
    ft_memset(t_array, 0, sizeof(t_ta));
    t_array->tokens = malloc(sizeof(char *) * 10);
    if (!t_array->tokens)
    {
        free(t_array);
        return (NULL);
    }
    t_array->quoted = alloc_quoted(10);
    if (!t_array->quoted)
    {
        free(t_array->tokens);
        free(t_array);
        return (NULL);
    }
    tokenarray_init_second(t_array);
    return (t_array);
}
int	tokenarray_resize(t_ta *t_array, int new_capacity)
{
    char	**new_tokens;
    int		*new_quoted;
    int		i;

    if (!t_array || new_capacity <= t_array->cap)
        return (0);
    new_tokens = malloc(sizeof(char *) * new_capacity);
    new_quoted = alloc_quoted(new_capacity);
    if (!new_tokens || !new_quoted)
    {
        free(new_tokens);
        free(new_quoted);
        return (0);
    }
    i = 0;
    while (i < t_array->t_tot)
    {
        new_tokens[i] = t_array->tokens[i];
        new_quoted[i] = t_array->quoted[i];
        i++;
    }
    free(t_array->tokens);
    free(t_array->quoted);
    t_array->tokens = new_tokens;
    t_array->quoted = new_quoted;
    t_array->cap = new_capacity;
    return (1);
}
t_cmd	*cmd_initialisation(void)
{
    t_cmd	*cmd;

    cmd = malloc(sizeof(t_cmd));
    if (!cmd)
        return (NULL);
    ft_memset(cmd, 0, sizeof(t_cmd));
    cmd->tty_backup = -1;
    cmd->heredoc_fd = -1;
    
    return (cmd);
}
