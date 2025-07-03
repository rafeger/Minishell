#include "../../include/minishell.h"

// --- Helpers ---

static int	is_quote_char(char c)
{
    return (c == '\'' || c == '"');
}

static int	is_special_char(char c)
{
    return (c == '<' || c == '>' || c == '|' || c == '$');
}

static void	ensure_token_capacity(t_ta *lx)
{
    size_t	oldsize;
    char	*tmp;

    oldsize = lx->tokensize + 1;
    if (lx->tokenindex == lx->tokensize)
    {
        if (lx->tokensize == 0)
            lx->tokensize = 1;
        else
            lx->tokensize *= 2;
        tmp = ft_realloc(lx->token, oldsize * sizeof(char),
                (lx->tokensize + 1) * sizeof(char));
        if (!tmp)
            return ;
        lx->token = tmp;
        lx->token[lx->tokenindex] = '\0';
    }
}

static void	expand_token_array(t_ta *lx)
{
    int		new_cap;
    char	**new_tokens;
    int		*new_quoted;

    new_cap = lx->cap * 2;
    new_tokens = ft_realloc(lx->tokens, lx->cap * sizeof(char *),
            new_cap * sizeof(char *));
    new_quoted = ft_realloc(lx->quoted, lx->cap * sizeof(int),
            new_cap * sizeof(int));
    if (!new_tokens || !new_quoted)
        return ;
    lx->tokens = new_tokens;
    lx->quoted = new_quoted;
    lx->cap = new_cap;
}

static int	add_token_failed_new(t_ta *lx)
{
    free(lx->tokens);
    free(lx->quoted);
    lx->tokens = NULL;
    lx->quoted = NULL;
    return (1);
}

static int	add_token_new(t_ta *lx, char *token)
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

static void	append_trailing_space_new(t_ta *lx, int was_quoted)
{
    char	*tmp;

    if (lx->tokenindex == 0)
    {
        lx->token[0] = ' ';
        lx->token[1] = '\0';
        add_token_new(lx, lx->token);
    }
    else
    {
        tmp = ft_strjoin(lx->token, " ");
        if (tmp)
        {
            lx->quoted[lx->t_tot] = was_quoted;
            add_token_new(lx, tmp);
            free(tmp);
        }
    }
    lx->trailing_space = 0;
}

static void	finalize_token_new(t_ta *lx)
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

static void	quote_state_machine_new(t_ta *lx, char c)
{
    int	was_quoted;

    if (is_quote_char(c))
    {
        if (!lx->in_q)
        {
            lx->in_q = 1;
            lx->quotechar = c;
            lx->second_quote = 1;
        }
        else if (c == lx->quotechar)
        {
            was_quoted = lx->in_q;
            lx->in_q = 0;
            lx->quotechar = '\0';
            lx->second_quote = was_quoted;
        }
        else
            lx->token[lx->tokenindex++] = c;
    }
    else
        lx->token[lx->tokenindex++] = c;
}

static void	handle_quotes_new(t_ta *lx, char **input)
{
    if ((!lx->in_q) && ((**input == '"' && *(*input + 1) == '"') ||
            (**input == '\'' && *(*input + 1) == '\'')))
    {
        if (*(*input + 2) && ft_isspace(*(*input + 2)))
            lx->trailing_space = 1;
        *input += 1;
        return ;
    }
    quote_state_machine_new(lx, **input);
    if (!lx->in_q && *(*input + 1) && ft_isspace(*(*input + 1)))
        lx->trailing_space = 1;
}

static void	handle_special_token_new(t_ta *lx, char **input)
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

static void	handle_special_chars_new(t_ta *lx, char **input)
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

// --- Main lexing step ---
static void	lex_step_new(t_ta *lx, char **input)
{
    ensure_token_capacity(lx);
    if (is_quote_char(**input) || lx->in_q)
        handle_quotes_new(lx, input);
    else if (**input == ' ' || **input == '\t')
        finalize_token_new(lx);
    else if (!lx->in_q && is_special_char(**input))
        handle_special_chars_new(lx, input);
    else
        lx->token[lx->tokenindex++] = **input;
}

// --- Main lexing loop ---
static void	lex_input_new(t_ta *lx, char *input)
{
    while (*input)
    {
        lex_step_new(lx, &input);
        input++;
    }
    finalize_token_new(lx);
}

// --- Entry point ---
t_ta	*new_lexer(char *input, t_shell_data *shell_data)
{
    t_ta	*lx;

    lx = tokenarray_init();
    if (!lx)
        return (NULL);
    if (is_only_quotes(input))
        return (create_special_empty_token(lx));
    lex_input_new(lx, input);
    if (check_unclosed_quotes(lx, shell_data))
        return (clean_lexer(lx));
    if (lx->tokenindex > 0)
        lx->token[lx->tokenindex] = '\0';
    if (lx->tokenindex > 0 && add_token_new(lx, lx->token))
        return (clean_lexer(lx));
    if (!lx->t_tot)
        return (clean_lexer(lx));
    return (lx);
}

int	is_only_quotes(const char *input)
{
    int	i;
    int	quotes;

    i = 0;
    quotes = 0;
    while (input[i])
    {
        if (input[i] == '\'' || input[i] == '"')
            quotes++;
        else if (input[i] != ' ' && input[i] != '\t')
            return (0);
        i++;
    }
    return (quotes > 0);
}

t_ta	*create_special_empty_token(t_ta *ta)
{
    ta->tokens[0] = ft_strdup("");
    ta->quoted[0] = 1;
    ta->t_tot = 1;
    return (ta);
}

int	check_unclosed_quotes(t_ta *ta, t_shell_data *shell_data)
{
    if (ta->in_q)
    {
        write(2, "minishell: syntax error: unclosed quote\n", 40);
        if (shell_data)
            shell_data->last_exit_status = 2;
        return (1);
    }
    return (0);
}

t_ta	*clean_lexer(t_ta *ta)
{
    free_tokenarray(ta);
    return (NULL);
}