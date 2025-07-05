/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_all.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rafeger <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/05 11:07:58 by rafeger           #+#    #+#             */
/*   Updated: 2025/07/05 11:08:00 by rafeger          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	free_env_array(char **env_array)
{
	int	i;

	if (!env_array)
		return ;
	i = 0;
	while (env_array[i])
	{
		free(env_array[i]);
		i++;
	}
	free(env_array);
}

void	free_tokenarray(t_ta *t_a)
{
	int	i;

	if (!t_a)
		return ;
	if (t_a->tokens)
	{
		i = 0;
		while (i < t_a->t_tot)
		{
			if (t_a->tokens[i])
				free(t_a->tokens[i]);
			i++;
		}
		free(t_a->tokens);
	}
	if (t_a->quoted)
		free(t_a->quoted);
	if (t_a->token)
		free(t_a->token);
	free(t_a);
}

void	free_redirects(t_redirect *redirect)
{
	t_redirect	*temp;

	while (redirect)
	{
		temp = redirect->next;
		free(redirect->file);
		free(redirect);
		redirect = temp;
	}
}
