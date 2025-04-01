/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zmurie <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 11:14:46 by zmurie            #+#    #+#             */
/*   Updated: 2025/03/18 11:14:48 by zmurie           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../minishell.h"

static void	update_pwd(char *arg, t_env *env)
{
	t_env	*tmp_env;
	char	cwd[PATH_MAX];
	char	*pwd;

	if (getcwd(cwd, PATH_MAX) == NULL)
	{
		perror(arg);
		return ;
	}
	tmp_env = env;
	while (ft_strncmp("PWD=", tmp_env->str, 4))
		tmp_env = tmp_env->next;
	pwd = ft_strjoin("PWD=", cwd);
	if (!pwd)
	{
		perror("malloc");
		return ;
	}
	tmp_env->str = pwd;
}

static void	update_old_pwd(char *arg, t_env *env)
{
	t_env	*tmp_env;
	char	*old_pwd;
	char	*new_old_pwd;

	tmp_env = env;
	while (ft_strncmp("PWD=", tmp_env->str, 4))
		tmp_env = tmp_env->next;
	old_pwd = tmp_env->str;
	tmp_env = env;
	while (ft_strncmp("OLDPWD=", tmp_env->str, 7))
		tmp_env = tmp_env->next;
	new_old_pwd = ft_strjoin("OLD", old_pwd);
	if (!new_old_pwd)
	{
		perror("malloc");
		return ;
	}
	tmp_env->str = new_old_pwd;
	update_pwd(arg, env);
}

int	ft_cd(char **args, t_env *env)
{
	if (args && !args[1])
		return (0);
	if (args && args[1] && args[2])
	{
		ft_putstr_fd("bash: cd: too many arguments\n", 2);
		return (1);
	}
	if (!chdir(args[1]))
	{
		update_old_pwd(args[1], env);
		return (0);
	}
	else
	{
		ft_putstr_fd("bash: cd: ", 2);
		ft_putstr_fd(args[1], 2);
		ft_putstr_fd(": No such file or directory\n", 2);
		return (1);
	}
}

// int	main(void)
// {
// 	t_env	**env;
// 	char	**str = malloc(sizeof(char *)*2);

// 	env = malloc(4 * sizeof(t_env));
// 	env[0] = malloc(sizeof(t_env));
// 	env[1] = malloc(sizeof(t_env));
// 	env[2] = malloc(sizeof(t_env));
// 	env[0]->prev = NULL;
// 	env[0]->str ="USER=zmurie";
// 	env[0]->next = env[1];
// 	env[1]->prev = env[0];
// 	env[1]->str = "OLDPWD=/home/zmurie";
// 	env[1]->next = env[2];
// 	env[2]->prev = env[1];
// 	env[2]->str = "PWD=/home/zmurie/Minishell";
// 	env[2]->next = NULL;
// 	str[0] = "cd";
// 	str[1] = "builtin";
// 	ft_cd(str, *env);
// 	t_env *tmp = env[0];
// 	while(tmp)
// 	{
// 		printf("%s\n", tmp->str);
// 		tmp = tmp->next;
// 	}	
// 	return (0);
// }