#include "../../minishell.h"

char **append_arg(char **args, char *arg)
{
    int count = 0;
    while (args && args[count])
        count++;
    char **new_args = malloc(sizeof(char *) * (count + 2));
    if (!new_args)
        return NULL;
    int i = 0;
    while (i < count)
    {
        new_args[i] = args[i];
        i++;
    }
    new_args[count] = ft_strdup(arg);
    if (!new_args[count])
    {
        free(new_args);
        return NULL;
    }
    new_args[count + 1] = NULL;
    free(args);
    return (new_args);
}
t_command *create_command(void)
{
	t_command *cmd;
	cmd = calloc(1, sizeof(t_command));\
	if (!cmd)
		return NULL;
	cmd->infile = -1;
	cmd->outfile = -1;
	return cmd;
}
int handle_redirection(t_command *cmd, t_token **tok)
{
    t_token *redir = *tok;
    t_token *target = redir->next;

    if (!target || target->type != T_WORD)
    {
        printf(stderr)
    }
}

t_command *parse_single_command(t_token **tok)
{
    t_command *cmd = create_command();
    if (!cmd)
        return NULL;

    while (*tok && (*tok)->type != T_PIPE)
    {
        if ((*tok)->type == T_WORD)
        {
            cmd->args = append_arg(cmd->args, (*tok)->value);
            if (!cmd->args) {
                free_command_list(cmd);
                return NULL;
            }
        } 
        else if ((*tok)->type == T_REDIR_IN || (*tok)->type == T_REDIR_OUT 
        || (*tok)->type == T_REDIR_APPEND)
        {
            if (!handle_redirection(cmd, tok))
            {
                free_command_list(cmd);
                return NULL;
            }
        }
        *tok = (*tok)->next;
    }
    if (*tok && (*tok)->type == T_PIPE)
    {
        cmd->pipe = 1;
        *tok = (*tok)->next;
    }
    return cmd;
}

t_command *parse_tokens_to_command(t_token *tok)
{
    t_command *head = NULL;
    t_command *cur = NULL;

    while (tok)
    {
        t_command *cmd = parse_single_command(&tok);
        if (!cmd)
        {
            free_command_list(head);
            return NULL;
        }
        if (!head)
            head = cmd;
        else
            cur->next = cmd;
        cur = cmd;
    }

    return head;
}