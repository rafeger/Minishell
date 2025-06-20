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
# include <signal.h>
# include <stdlib.h>
# include "libft/libft.h"
# include <readline/readline.h>
# include <readline/history.h>

# define PROMPT "minishell> "

extern volatile sig_atomic_t	g_sig;


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
	struct s_env		*prev;
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
   



typedef struct s_exp
{
	char				*result;
	int					i;
	int					j;
	int					squote;
	int					dquote;
	t_shell_data		*shell;
}	t_exp;

// typedef struct s_command
// {
// 	char				**args;
// 	int					infile;
// 	int					outfile;
// 	int					append;
// 	char 				*heredoc_delim;
// 	struct s_command	*next;
// }	t_command;

extern pid_t	g_signal;

int		ft_cd(char **args, t_env *env);
int		ft_echo(char **arg);
int		ft_env(t_env *env);
void	ft_exit(t_shell_data *data, char **args);
int		check_valid_name_var(char *str);
int		ft_export(char **args, t_env **env);
int		ft_pwd(void);
int		ft_unset(char **args, t_env **env);
int		is_builtin(char *cmd);
void	do_builtin(t_shell_data *data, t_cmd *cmd);
char	*get_pathname(char *cmd, t_env *env);
char	**convert_list_to_tab_str(t_env *env);
void	free_all(t_shell_data *data);
int		ft_envsize(t_env *lst);


/*================================== lexer ===================================*/

/* lexer_core.c */
void			process_char(t_ta *ta, char **input);
void			process_input(t_ta *ta, char *input);
void			handle_token_end(t_ta *ta);
int				add_token(t_ta *ta, char *token);
t_ta			*lexer(char *input);

/* lexer_quote.c */
int				is_only_quotes(const char *input);
t_ta			*create_special_empty_token(t_ta *ta);
void			handle_empty_quotes(t_ta *ta, char **input);
void			process_quotes(t_ta *ta, char **input);
void			handle_quotes(t_ta *ta, char *input);

/* lexer_special.c */
void			handle_trailing_space(t_ta *ta, int was_quoted);
void			handle_special_chars(t_ta *ta, char **input);
void			resize_token_array(t_ta *ta);

/* lexer_util.c */
int				add_token_failed(t_ta *ta);
t_ta			*clean_lexer(t_ta *ta);
int				check_unclosed_quotes(t_ta *ta);

/*================================== parser ==================================*/

/* parse_args.c */
int				should_concat(char *prev_arg, char *curr_arg);
void			concat_argument(t_cmd *cmd, char *arg);
void			add_argument(t_cmd *cmd, char *arg, int quoted);

/* parse_core.c */
int				process_token(t_cmd *cmd, t_ta *ta, int *i);
void			handle_redirect(t_cmd *cmd, t_ta *ta, int *i);
int				handle_pipe_token(t_cmd *cmd, t_ta *ta, int *i);
void			handle_empty_token(t_cmd *cmd, t_ta *ta, int *i);

t_cmd			*parse_tokens(t_ta *ta);

/* parse_redirect.c */
void			cleanup_pipe_data(t_ta *new_ta, char **sub_tokens, \
					int last_alloc);
int				is_redirect(const char *token);
int				get_redirect_type(char *token);
void			add_redirect(t_cmd *cmd, int type, char *file, int eof_quoted);

/* tokenarray_utils.c */
t_ta			*init_new_ta(t_ta *ta, int index);
char			**create_sub_tokens(t_ta *ta, int index, t_ta *new_ta);
int				init_quoted_array(t_ta *new_ta, t_ta *ta, int index);

/*================================== expand ==================================*/

/* expand_core.c */
int				get_var_length(const char *str);
char			*get_var_value(const char *var, t_shell_data *shell_data);
void			process_expand_char(t_exp *exp, char *input);
size_t			calculate_expanded_size(char *input, t_shell_data *shell_data);
char			*expand_variables(char *input, t_shell_data *shell_data);

/* expand_size_utils.c */
int				handle_quoted_len(char **result, int *j, char *input, \
					int quote_len);
int				process_var(char *input, int i, size_t *size, t_shell_data *sd);

/* expand_var_utils.c */
int				is_in_quotes(const char *str);
void			copy_var_value(char **result, int *j, char *var_value);
int				get_quoted_length(const char *str);
int				handle_quoted_var(char **result, int *j, char *input);
int				handle_var(char **res, int *j, char *in, t_shell_data *sd);

/*=============================== init_and_free ==============================*/

/* free_core.c */
void			free_ptr(void *ptr);
void			free_env_array(char **env_array);
void			free_tokenarray(t_ta *ta);
void			free_redirects(t_redirect *redirect);

/* tokenarray_init.c */
void			tokenarray_init_second(t_ta *ta);
t_ta			*tokenarray_init(void);

/* struct_init_core.c */
t_cmd			*cmd_init(void);
void			fd_info_init(t_cmd *cmd);

/* free_advanced.c */
void			free_command_args(t_cmd *cmd);
void			free_command(t_cmd *cmd);
void			ft_cleanup_shell(t_shell_data **shell);
void			ft_cleanup_env(t_env **env);

/*=================================== env ====================================*/

/* signal_core.c */
void			sigint_handler(int sig);
void			init_signals(void);
void			update_exit_status(t_shell_data *shell_data);

/* env_core.c */
char			*create_env_string(const char *key, const char *value);
void			*get_env_value(t_env *env, const char *key);
void			set_env_value(t_env **env, const char *key, const char *value);
void			remove_env_var(t_env **env, const char *key);
char			**env_list_to_array(t_env *env);

/* env_init.c */
char			**allocate_env_array(t_env *env, int *count);
int				fill_env_array(char **env_array, t_env *env);
t_env			*create_env_node(char *env_str);
void			add_env_node(t_env **env_list, t_env *new_node);
t_env			*init_env(char **envp);

/* shell_core.c */
t_shell_data	*init_shell_data(char **envp);
int				initialize_shell(t_shell_data **shell, char **envp);
void			update_shlvl(t_env **env, int level);
void			initialize_shlvl(t_env **env);


#endif

