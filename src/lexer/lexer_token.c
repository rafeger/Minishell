#include "../../include/minishell.h"

int	add_token_failed_new(t_ta *lx)
{
    free(lx->tokens);
    free(lx->quoted);
    lx->tokens = NULL;
    lx->quoted = NULL;
    return (1);
}

int	add_token_new(t_ta *lx, char *token)
{
    char	**new_tokens;
    int		*new_quoted;
    size_t	oldsize;

    if (lx->t_tot == lx->cap)
    {
        oldsize = lx->cap;
        lx->cap *= 2;
        new_tokens = ft_realloc(lx->tokens, oldsize * sizeof(char *),
                sizeof(char *) * lx->cap);
        new_quoted = ft_realloc(lx->quoted, oldsize * sizeof(int),
                sizeof(int) * lx->cap);
        if (!new_tokens || !new_quoted)
            return (add_token_failed_new(lx));
        lx->tokens = new_tokens;
        lx->quoted = new_quoted;
    }
    lx->tokens[lx->t_tot] = ft_strdup(token);
    if (!lx->tokens[lx->t_tot])
        return (1);
    lx->quoted[lx->t_tot] = lx->second_quote;
    lx->t_tot++;
    return (0);
}

void	append_trailing_space_new(t_ta *lx, int was_quoted)
{
    char	*tmp;

    if (lx->tokenindex == 0)
        return;
    tmp = ft_strjoin(lx->token, " ");
    if (tmp)
    {
        lx->quoted[lx->t_tot] = was_quoted;
        add_token_new(lx, tmp);
        free(tmp);
    }
    lx->trailing_space = 0;
}

void	finalize_token_new(t_ta *lx)
{
    int	was_quoted;

    was_quoted = lx->second_quote;
    if (lx->tokenindex > 0 || lx->trailing_space)
    {
        if (lx->t_tot >= lx->cap)
            expand_token_array(lx);
        lx->token[lx->tokenindex] = '\0';
        if (lx->trailing_space && lx->tokenindex == 0)
            append_trailing_space_new(lx, was_quoted);
        else
        {
            lx->quoted[lx->t_tot] = was_quoted;
            add_token_new(lx, lx->token);
        }
        lx->tokenindex = 0;
        lx->second_quote = 0;
    }
}