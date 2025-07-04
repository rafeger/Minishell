#include "../../include/minishell.h"

static void	clear_current_command(t_shell_data *shell_data)
{
	if (shell_data->cmd)
	{
		free_command(shell_data->cmd);
		shell_data->cmd = NULL;
	}
}

static int	perform_syntax_validation(char *input, t_shell_data *shell_data)
{
	if (shell_syntax_check(input) != 0)
	{
		shell_data->last_exit_status = 2;
		free(input);
		return (1);
	}
	return (0);
}

static t_cmd	*prepare_and_execute_input(char *input, t_shell_data *shell_data)
{
	char	*expanded_input;
	t_ta	*t_array;
	t_cmd	*cmd;
	int		is_empty;

	expanded_input = expand_variables(input, shell_data);
	if (!expanded_input)
		return (NULL);
	is_empty = (expanded_input[0] == '\0');
	t_array = lexer(expanded_input, shell_data);
	free(expanded_input);
	if (!t_array && is_empty)
	{
		shell_data->last_exit_status = 0;
		return (NULL);
	}
	if (!t_array)
		return (NULL);
	cmd = parse_tokens(t_array);
	free_tokenarray(t_array);
	return (cmd);
}

static int setup_heredocs(t_shell_data *data)
{
    t_cmd *cmd = data->cmd;
    
    while (cmd)
    {
        t_redirect *redir = cmd->redirects;
        while (redir)
        {
            if (redir->type == HERE_DOC)
            {
                cmd->heredoc_fd = heredoc(redir->file, data);
                if (cmd->heredoc_fd == -1)
                {
                    if (data->last_exit_status == 130)
                        return (-1);
                    return (-1);
                }
            }
            redir = redir->next;
        }
        cmd = cmd->next;
    }
    return (0);
}

static void	run_command_if_valid(t_cmd *cmd, t_shell_data *sd)
{
	if (cmd)
	{
		sd->cmd = cmd;
		if (setup_heredocs(sd) == -1)
		{
			free_command(cmd);
			sd->cmd = NULL;
			return;
		}
		execute_commands(sd);
	}
}
// static void print_cmd_args(t_cmd *cmd)
// {
//     int i;
//     t_cmd *cur = cmd;
//     int cmd_num = 1;
//     t_redirect *redir;

//     while (cur)
//     {
//         printf("---- Command %d ----\n", cmd_num++);
//         if (cur->c_name)
//             printf("c_name: [%s] (quoted: %d)\n", cur->c_name, cur->quoted);
//         else
//             printf("c_name: [NULL]\n");
//         for (i = 0; i < cur->arg_count; i++)
//             printf("arg[%d]: [%s] (quoted: %d)\n", i, cur->args[i], cur->arg_quoted[i]);
//         redir = cur->redirects;
//         while (redir)
//         {
//             const char *type_str = "UNKNOWN";
//             if (redir->type == REDIR_IN)
//                 type_str = "<";
//             else if (redir->type == REDIR_OUT)
//                 type_str = ">";
//             else if (redir->type == APPEND)
//                 type_str = ">>";
//             else if (redir->type == HERE_DOC)
//                 type_str = "<<";
//             printf("redir: [%s] file: [%s]\n", type_str, redir->file);
//             redir = redir->next;
//         }
//         printf("-------------------\n");
//         cur = cur->next;
//     }
// }

void	process_shell_input(char *input, t_shell_data *shell_data)
{
	t_cmd	*cmd;

	if (!input)
		exit(0);
	if (perform_syntax_validation(input, shell_data))
		return ;
	clear_current_command(shell_data);
	cmd = prepare_and_execute_input(input, shell_data);
    // print_cmd_args(cmd);
	free(input);
	run_command_if_valid(cmd, shell_data);
}
