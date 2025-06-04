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
# include <readline/readline.h>
# include <readline/history.h>

# define PROMPT "minishell> "


typedef struct s_fd_info
{
	int					stdin_backup;
	int					stdout_backup;
	int					pipe_fd[2];
}	t_fd_info;


// typedef struct s_env
// {
// 	char			*str;
// 	struct s_env	*prev;
// 	struct s_env	*next;
// }					t_env;


/* *key : key of the environment variable.
   *value : value of the environment variable.
   *next : pointer the the next environment variable. */
typedef struct s_env
{
	char				*key;
	char				*value;
	struct s_env		*next;
}	t_env;


/*	token array
	**tokens : an array of strings containing each individual tok
	*token	: temp buffer to construct each tok individually
	quotechar : contains the type of quote (simple or double)
	token index : keep track of curent pos in the token buffer
	count : count the total number of toks
	inquotes : flag if in quotes duh
	tokensize : contains the len of said tok
	capacity : total number of toks we can handle dinamically*/

typedef struct s_ta
{
	char				**tokens;
	char				*token;
	char				quotechar;
	size_t				tokenindex;
	size_t				tokensize;
	int					inquotes;
	int					count;
	int					capacity;
	int					trailing_space;
	int					*quoted;
	int					second_quote;
}	t_ta;

/* type : type of redirection, defined by 0 = <, 1 = >, 2 = << and 3 = >>.
   *file : it's the name of the file implicated in the redirection.
   *next : pointer to the next redirection. */
typedef struct s_redirect
{
	int					type;
	int					eof_quoted;
	char				*file;
	struct s_redirect	*next;
}	t_redirect;


/* **args : array of the command's arguments.
   *name : name of the command.
   arg_count : number of arguments.
   *redirects : chained list of the redirections associated to the command.
   *next : pointer to the next command in case of pipes. */
typedef struct s_cmd
{
	int					arg_count;
	char				**args;
	char				*name;
	t_redirect			*redirects;
	struct s_cmd		*next;
	struct s_cmd		*prev;
	t_fd_info			*fd_info;
	int					n_quoted;
	pid_t				pid;
	int					tty_backup;
	int					has_heredoc;
	int					heredoc_fd;
	int					has_next;
	int					quoted;
	int					*arg_quoted;
}	t_cmd;

/* *env : chained list of the environment variable.
   *command :
 last_exit_status : output status of the last executed command. */
typedef struct s_shell_data
{
	t_cmd				*cmd;
	t_env				*env;
	int					last_exit_status;
	int					sig_quit_flag;
}	t_shell_data;
   

// typedef struct s_command
// {
// 	char				**args;
// 	int					infile;
// 	int					outfile;
// 	int					append;
// 	char 				*heredoc_delim;
// 	struct s_command	*next;
// }	t_command;


typedef struct s_exp
{
	char				*result;
	int					i;
	int					j;
	int					squote;
	int					dquote;
	t_shell_data		*shell;
}	t_exp;

typedef struct s_data
{
	t_env		*env;
	t_cmd		*cmd;
	int			sig_quit_flag;
	int			last_exit_status;
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
void	do_builtin(t_data *data, t_cmd *cmd);
char	*get_pathname(char *cmd, t_env *env);
char	**convert_list_to_tab_str(t_env *env);
void	free_all(t_data *data);
int		ft_envsize(t_env *lst);


#endif

