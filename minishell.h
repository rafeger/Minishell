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


typedef enum	e_token_type
{
	T_WORD = 1,
	T_PIPE,
	T_REDIR_IN,
	T_REDIR_OUT,
	T_REDIR_APPEND,
	T_REDIR_HEREDOC,
	T_INVALID
}	t_token_type;

typedef	struct s_token
{
	t_token_type type;
	char		*value;
	struct s_token *next;
}	t_token;


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
	char 				*heredoc_delim;
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
char	*get_pathname(char *cmd, t_env *env);
char	**convert_list_to_tab_str(t_env *env);
void	free_all(t_data *data);
int		ft_envsize(t_env *lst);

//misc_tokens.c
bool	ft_isspace(char c);
void	handle_whitespaces(char *str, int *i);
char	**list_to_str_array(t_list *lst);

//free.c
void    free_command_list(t_command *cmd);
void	free_token_list(t_token *tok);
void	free_args(char **args);
void	free_str_list(t_list *lst);

//toke_utils.c
char	*ft_strncpy(char *dest, const char *src, int n);
int		print_syntax_error(const char *msg);
int		check_syntax_errors(t_token *head);
char *ft_strncpy(char *dest, const char *src, int n);
int	print_syntax_error(const char *msg);
<<<<<<< HEAD
=======
int	check_syntax_errors(t_token *head);
>>>>>>> origin

//tokenizer.c
t_token	*tokenize(char *input);

//extract_token.c
char	*extract_token(const char *str, int *i);

//crea_cmd.c

int		handle_redirection(t_command *cmd, t_token **tok);
t_command	*parse_tokens(t_token *tokens);

//expand.c
char	*expand_variables(const char *str);

//append_args.c
int	fill_command(t_command *cmd, t_token **tok);

//debug.c
void	print_tokens(t_token *tok);
void 	print_commands(t_command *cmd);

//main
void	init_command(t_command *cmd);

#endif

