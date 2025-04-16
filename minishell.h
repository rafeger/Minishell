#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <stdbool.h>
# include <limits.h>
# include <unistd.h>
# include <string.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <fcntl.h>
# include <stdlib.h>
# include "libft/libft.h"

typedef struct s_env
{
	char			*str;
	struct s_env	*prev;
	struct s_env	*next;
}					t_env;

typedef struct s_command
{
	char				**args;
	int					infile;
	int					outfile;
	int					append;
	int					pipe;
	struct s_command	*next;
}	t_command;

typedef struct s_data
{
	t_env		*env;
	t_command	*command;
	int			exit;

}				t_data;

extern pid_t	g_signal;

int		ft_cd(char **args, t_env *env);
int		ft_echo(char **arg);
int		ft_env(t_env *env);
void	ft_exit(t_data *data, char **args);
int		check_valid_name_var(char *str);
int		ft_export(char **args, t_env **env);
int		ft_pwd(void);
int		ft_unset(char **args, t_env **env);
int		is_builtin(char *cmd);
void	do_builtin(t_data *data, t_command *cmd);
#endif

