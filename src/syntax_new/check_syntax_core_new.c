#include "../../include/minishell.h"

static int	skip_blanks(const char *str, int idx)
{
    while (str[idx] && ft_isspace(str[idx]))
        idx++;
    return (idx);
}

static int	is_it_in_q(const char *str, int idx)
{
    int		pos;
    int		in_quotes;
    char	quote_type;

    pos = 0;
    in_quotes = 0;
    quote_type = 0;
    while (pos <= idx)
    {
        if ((str[pos] == '\'' || str[pos] == '"') && !in_quotes)
        {
            in_quotes = 1;
            quote_type = str[pos];
        }
        else if (str[pos] == quote_type && in_quotes)
        {
            in_quotes = 0;
            quote_type = 0;
        }
        pos++;
    }
    return (in_quotes);
}

static int	redir_overflow(const char *line, int pos)
{
    int	n;

    n = 1;
    while (line[pos + n] && line[pos + n] == line[pos])
        n++;
    if (n > 2)
    {
        ft_putstr_fd("minishell: ", 2);
        if (line[pos] == '>')
        {
            if (n > 3)
                ft_putendl_fd("NOOOOOOOOOOOOOOOOOOOOOOo", 2);
            else
                ft_putendl_fd("NOOOOOOOOOOOOOOOOOOOOOOo", 2);
        }
        else
        {
            if (n > 3)
                ft_putendl_fd("sNOOOOOOOOOOOOOOOOOOOOOOo", 2);
            else
                ft_putendl_fd("NOOOOOOOOOOOOOOOOOOOOOOo", 2);
        }
        return (1);
    }
    return (0);
}

static int	redir_missing_target(const char *line, int pos)
{
    int	next;

    next = skip_blanks(line, pos + 1);
    if (!line[next])
    {
        ft_putstr_fd("minishell: ", 2);
        ft_putendl_fd("syntax error near unexpected token 'newline'", 2);
        return (1);
    }
    if ((line[next] == '<' || line[next] == '>') && !(line[next] == line[pos] && next == pos + 1))
    {
        ft_putstr_fd("minishell: syntax error near unexpected token '", 2);
        ft_putchar_fd(line[next], 2);
        ft_putendl_fd("'", 2);
        return (1);
    }
    return (0);
}

static int	redir_invalid_combo(const char *line, int pos)
{
    int		j;
    char	first;
    char	second;

    j = pos + 1;
    first = line[pos];
    while (line[j] && ft_isspace(line[j]))
        j++;
    if (line[j] && (line[j] == '<' || line[j] == '>'))
    {
        // Allow << and >> (heredoc and append)
        if (line[j] == first && j == pos + 1)
            return (0); // << or >> is valid
        second = line[j];
        ft_putstr_fd("minishell: syntax error near unexpected token '", 2);
        if (first == second)
        {
            ft_putchar_fd(second, 2);
            ft_putchar_fd(first, 2);
        }
        else
            ft_putchar_fd(second, 2);
        ft_putendl_fd("'", 2);
        return (1);
    }
    return (0);
}

static int	redir_pipe_error(const char *line, int pos)
{
    int	next;

    next = pos + 1;
    while (line[next] && ft_isspace(line[next]))
        next++;
    if (line[next] == '|')
    {
        ft_putendl_fd("minishell: syntax error near unexpected token '|'", 2);
        return (1);
    }
    return (0);
}

static int	pipe_at_begin(const char *line)
{
    int	idx;

    idx = skip_blanks(line, 0);
    if (line[idx] == '|')
    {
        ft_putstr_fd("minishell: ", 2);
        if (line[idx + 1] == '|')
            ft_putendl_fd("syntax error near unexpected token '||'", 2);
        else
            ft_putendl_fd("syntax error near unexpected token '|'", 2);
        return (1);
    }
    return (0);
}

static int	pipe_sequence_error(const char *line, int *idx)
{
    int	count;
    int	trailing;

    count = 1;
    trailing = *idx + 1;
    while (line[*idx + count] && line[*idx + count] == '|')
        count++;
    if (count > 1)
    {
        ft_putendl_fd("minishell: syntax error near unexpected token '||'", 2);
        *idx += count - 1;
        return (1);
    }
    while (line[trailing] && ft_isspace(line[trailing]))
        trailing++;
    if (!line[trailing] || line[trailing] == '|')
    {
        ft_putendl_fd("minishell: syntax error near unexpected token '|'", 2);
        return (1);
    }
    return (0);
}

int	shell_syntax_check(const char *line)
{
    int	k;

    k = 0;
    if (!line)
        return (1);
    if (pipe_at_begin(line))
        return (1);
    while (line[k])
    {
        if (line[k] == '|' && !is_it_in_q(line, k))
        {
            if (pipe_sequence_error(line, &k))
                return (1);
        }
        if ((line[k] == '>' || line[k] == '<') && !is_it_in_q(line, k))
        {
            // Handle double redir (<< or >>)
            if (line[k] == line[k + 1] && !is_it_in_q(line, k + 1))
            {
                if (redir_pipe_error(line, k))
                    return (1);
                if (redir_overflow(line, k))
                    return (1);
                if (redir_missing_target(line, k))
                    return (1);
                // skip redir_invalid_combo for << or >>
                k += 2;
                while (line[k] && ft_isspace(line[k]))
                    k++;
                continue;
            }
            // Single redir
            if (redir_pipe_error(line, k))
                return (1);
            if (redir_overflow(line, k))
                return (1);
            if (redir_missing_target(line, k))
                return (1);
            if (redir_invalid_combo(line, k))
                return (1);
        }
        k++;
    }
    return (0);
}