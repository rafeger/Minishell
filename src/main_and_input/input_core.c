#include "../../include/minishell.h"

/*
 * Safely frees and nullifies the current command structure in shell_data.
 * Essential for preventing memory leaks between command executions.
*/
static void	clear_current_command(t_shell_data *shell_data)
{
	if (shell_data->cmd)
	{
		free_command(shell_data->cmd);
		shell_data->cmd = NULL;
	}
}

/*
 * Validates input syntax using check_syntax().
 * If syntax errors are found:
 * - Sets error exit status.
 * - Frees input.
 * Returns 1 on syntax error, 0 if syntax is valid.
*/
static int	perform_syntax_validation(char *input, t_shell_data *shell_data)
{
	if (check_syntax(input) != 0)
	{
		shell_data->last_exit_status = 2;
		free(input);
		return (1);
	}
	return (0);
}

/*
 * Processes raw input into executable command:
 * - Expands variables.
 * - Performs lexical analysis.
 * - Parses tokens into command structure.
 * Returns NULL if any step fails, otherwise returns prepared command.
*/
static t_cmd	*prepare_and_execute_input(char *input, t_shell_data *shell_data)
{
	char	*expanded_input;
	t_ta	*ta;
	t_cmd	*cmd;
	int		is_empty;

	expanded_input = expand_variables(input, shell_data);
	if (!expanded_input)
		return (NULL);
	is_empty = (expanded_input[0] == '\0');
	ta = lexer(expanded_input);
	free(expanded_input);
	if (!ta && is_empty)
	{
		shell_data->last_exit_status = 0;
		return (NULL);
	}
	if (!ta)
		return (NULL);
	cmd = parse_tokens(ta);
	free_tokenarray(ta);
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
                    // Si le heredoc a été interrompu, on nettoie et on retourne
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

/*
 * Executes command if valid:
 * - Stores command in shell data.
 * - Triggers command execution.
 * No return value as execution status is stored in t_shell_data.
*/
static void	run_command_if_valid(t_cmd *cmd, t_shell_data *sd)
{
	if (cmd)
	{
		sd->cmd = cmd;
		// Si setup_heredocs retourne -1, on n'exécute pas la commande
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
//         if (cur->name)
//             printf("name: [%s] (quoted: %d)\n", cur->name, cur->quoted);
//         else
//             printf("name: [NULL]\n");
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


/*
 * Main input handling function that orchestrates:
 * - Syntax validation.
 * - Command cleanup.
 * - Input processing.
 * - Command execution.
 * Manages complete lifecycle of a single command input.
*/
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
