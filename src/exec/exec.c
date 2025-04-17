/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zmurie <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 14:39:34 by zmurie            #+#    #+#             */
/*   Updated: 2025/04/01 14:39:36 by zmurie           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../../minishell.h"
pid_t	g_signal;

static void	cleanup_split(char **array)
{
	int	i;

	i = 0;
	if (!array)
		return ;
	while (array[i])
		free(array[i++]);
	free(array);
}

static void	execute(t_data *data, t_command *cmd)
{
	char	*pathname;
	char	**commande;
	char	**tab_env;

	commande = ft_split(*cmd->args, ' ');
	pathname = get_pathname(commande[0], data->env);
	if (!pathname)
	{
		perror(pathname);
		data->exit = 127;
		cleanup_split(commande);
		return ;
	}
	if (access(pathname, X_OK))
	{
		perror(pathname);
		data->exit = 126;
		cleanup_split(commande);
		return ;
	}
	tab_env = convert_list_to_tab_str(data->env);
	if (execve(pathname, commande, tab_env) == -1)
	{
		perror("execve");
		free(pathname);
		cleanup_split(commande);
		exit(EXIT_FAILURE);
	}
}

static void	child_process(t_data *data, t_command *cmd, int *pipefd)
{
	close(pipefd[0]);
	if (cmd->infile >= 0)
	{
		dup2(cmd->infile, STDIN_FILENO);
		close(cmd->infile);
	}
	if (cmd->outfile >= 0)
	{
		dup2(cmd->outfile, STDOUT_FILENO);
		close(cmd->outfile);
	}
	close(pipefd[1]);
	execute(data, cmd);
}

static void	parent_process(t_command *cmd, int *pipefd)
{
	close(pipefd[1]);
	if (cmd->infile >= 0)
		close(cmd->infile);
	if (cmd->infile == -2)
		cmd->infile = pipefd[0];
	if (cmd->next->args && cmd->next->infile == -2)
		cmd->next->infile = pipefd[0];
	else
		close(pipefd[0]);
}

int	exec(t_data *data)
{
	int			*pipefd;
	t_command	*tmp_cmd;

	pipefd = NULL;
	tmp_cmd = data->command;
	while (tmp_cmd)
	{
		if (pipe(pipefd) == -1)
			return (1);
		g_signal = fork();
		if (g_signal == -1)
			exit(1);
		if (is_builtin(tmp_cmd->args[0]))
			do_builtin(data, tmp_cmd);
		if (g_signal == 0)
			child_process(data, tmp_cmd, pipefd);
		else
			parent_process(tmp_cmd, pipefd);
		tmp_cmd = tmp_cmd->next;
	}
	return (0);
}

int main(void)
{
	t_data data;
	t_command *cmd = malloc(sizeof(t_command ));
	char **params = malloc(sizeof(char *) * 3);
	t_env *env_node = malloc(sizeof(t_list));

	// Préparation d'une commande "echo hello"
	params[0] = strdup("echo");
	params[1] = strdup("hello");
	params[2] = NULL;

	// Remplir t_cmd
	cmd->infile = -1;
	cmd->outfile = -1;
	cmd->args = params;
	cmd->next = cmd;

	// Remplir un env minimal (PATH, par exemple)
	env_node->str = strdup("PATH=/usr/bin:/bin");
	env_node->prev = NULL;
	env_node->next = NULL;

	// Initialiser t_data
	data.env = env_node;
	data.command = cmd;
	data.exit = 0;

	// Lancer l'exécution
	exec(&data);

	// Afficher le code de sortie
	printf("exit_code = %d\n", data.exit);

	// Libération
	free(params[0]);
	free(params[1]);
	free(params);
	free(cmd);
	free(env_node->str);
	free(env_node);

	return (0);
}