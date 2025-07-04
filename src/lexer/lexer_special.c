#include "../../include/minishell.h"

void	handle_special_token_new(t_ta *lx, char **input)
{
    if (lx->tokensize < 2)
    {
        size_t	oldsize = lx->tokensize + 1;
        lx->tokensize = 2;
        lx->token = ft_realloc(lx->token, oldsize * sizeof(char),
                (lx->tokensize + 1) * sizeof(char));
        if (!lx->token)
            return ;
    }
    if ((**input == '<' && *(*input + 1) == '<') ||
        (**input == '>' && *(*input + 1) == '>'))
    {
        lx->token[1] = **input;
        lx->token[2] = '\0';
        (*input)++;
    }
    else
        lx->token[1] = '\0';
    add_token_new(lx, lx->token);
}

void	handle_special_chars_new(t_ta *lx, char **input)
{
    char	*spe;

    spe = "<>|$";
    if (lx->second_quote)
    {
        lx->token[lx->tokenindex++] = **input;
        return ;
    }
    if (ft_strchr(spe, **input))
    {
        if (lx->tokenindex > 0)
        {
            lx->token[lx->tokenindex] = '\0';
            add_token_new(lx, lx->token);
            lx->tokenindex = 0;
        }
        lx->token[0] = **input;
        handle_special_token_new(lx, input);
    }
}