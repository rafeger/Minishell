/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dollars.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pfranke <pfranke@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/08 02:31:41 by pfranke           #+#    #+#             */
/*   Updated: 2025/03/14 14:14:11 by pfranke          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DOLLARS_H
# define DOLLARS_H
# include "../minishell.h"

typedef struct s_dl
{
	int		i[4];
	int		total_size;
	char	*new;
	t_env	*env;
	t_data	*data;
	char	*str;
}	t_dl;
char	*dl_init(char *str, t_env *env, t_data *data);
void	handle_variable(t_dl *dl);
void	handle_special(t_dl *dl);
void	handle_dollar_init(t_dl *dl);
int		handle_dollar(t_dl *dl);
void	process_string(t_dl *dl);
void	append_and_update(t_dl *dl, char *src);
void	handle_quotes(t_dl *dl);
char	*makestrfromstartandend(char *str, int start, int end);
char	*get_variable_name(t_dl *dl);
int		dl_getsize(t_dl *dl);

#endif