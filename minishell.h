#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <stdbool.h>
# include <limits.h>
# include <unistd.h>
# include "libft/libft.h"

typedef struct s_env
{
	char			*str;
	struct s_env	*prev;
	struct s_env	*next;
}					t_env;

typedef struct s_data
{
	t_env	*env;
	int		exit;

}				t_data;

int		ft_cd(char **args, t_env *env);
int		ft_echo(char **arg);
int		ft_env(t_env *env);
void	ft_exit(t_data *data, char **args);
int		check_valid_name_var(char *str);
int		ft_export(char **args, t_env **env);
int		ft_pwd(void);
int		ft_unset(char **args, t_env **env);
#endif

