#include "../../minishell.h"

int	fill_command(t_command *cmd, t_token **tok)
{
	t_list	*arg_list = NULL;

	while (*tok && (*tok)->type != T_PIPE)
	{
		if ((*tok)->type == T_WORD)
		{
			char *expanded = expand_variables((*tok)->value);
			if (!expanded)
				return (free_str_list(arg_list), 0);
			ft_lstadd_back(&arg_list, ft_lstnew(expanded));
		}
		else if (!handle_redirection(cmd, tok))
		{
			free_str_list(arg_list);
			return (0);
		}
		*tok = (*tok)->next;
	}
	cmd->args = list_to_str_array(arg_list);
	free_str_list(arg_list);
	return (1);
}

