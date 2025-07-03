#ifndef MINISHELL_H
# define MINISHELL_H


# include "../libft/libft.h"


# include <stdio.h>
# include <stdbool.h>
# include <limits.h>
# include <unistd.h>
# include <string.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <sys/ioctl.h>
# include <sys/stat.h>
# include <fcntl.h>
# include <signal.h>
# include <stdlib.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <errno.h>

# define PROMPT "minishell> "
# define REDIR_IN    0
# define REDIR_OUT   1
# define HERE_DOC    2
# define APPEND      3


extern volatile sig_atomic_t	g_sig;


typedef struct s_fd_info
{
	int					stdin_backup;
	int					stdout_backup;
	int					pipe_fd[2];
}	t_fd_info;


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
	t_tot : count the total number of toks
	in_q : is a boolean (1 true / 0 false) to know if ur inside quotes or nah
	tokensize : contains the len of said tok
	cap : total number of toks we can handle dinamically*/

typedef struct s_ta
{
	char				**tokens;
	char				*token;
	char				quotechar;
	size_t				tokenindex;
	size_t				tokensize;
	bool				in_q;
	int					t_tot;
	int					cap;
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
	bool				heredoc_quotes;
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

typedef struct s_heredoc_data
{
	int					fd;
	char				*temp;
	char				*delimiter;
	int					expand;
	t_shell_data		*sd;
}	t_heredoc_data;

 typedef enum e_quote_state
{
    STATE_NONE,
    STATE_IN_SINGLE_QUOTE,
    STATE_IN_DOUBLE_QUOTE
} t_quote_state;

/*============================== builtin_zoe =================================*/
int		ft_cd(t_cmd *cmd, t_shell_data *shell_data);

int		ft_echo(t_cmd *cmd);

int		ft_env(t_shell_data *shell_data);

void	ft_exit(t_cmd *cmd, t_shell_data *data);

int		not_valid_id(char *str);

int		check_valid_value(char *str);

char	*get_value(char *str);

int		syntax_error(char c);

int		ft_export(t_cmd *cmd, t_shell_data *shell_data);

int		name_var_exist(char *str, t_env *env, bool *concat);

int		ft_pwd(void);

int		ft_unset(t_cmd *cmd, t_shell_data *shell_data);

/*============================== exec_zoe ====================================*/
int		is_builtin(char *cmd);

int		do_builtin(t_shell_data *data, t_cmd *cmd);

int		execute_commands(t_shell_data *data);

int		is_builtin_no_fork(char *cmd);

void	handle_builtin_child(t_shell_data *data, t_cmd *cmd);

int		handle_fork_error(t_cmd *cmd, int pipe_fd[2], t_shell_data *data);

void	wait_for_last_command(t_cmd *cmd, pid_t pid, int *status,
	t_shell_data *data);

void	close_heredoc_fds(t_cmd *cmd_list);

void	handle_parent_cleanup(t_cmd *cmd, int pipe_fd[2], int *input_fd);

int		create_pipe_if_needed(t_cmd *cmd, int pipe_fd[2], int *output_fd);

void	setup_pipes_and_redirections(t_cmd *cmd, int pipe_in, int pipe_out);

int		just_redir(t_shell_data *data, t_redirect *redir);

void	execute(t_shell_data *data, t_cmd *cmd);

char	*get_pathname(char *cmd, t_env *env);

int		heredoc(char *delim, t_shell_data *data);

void	setup_heredoc_signals(struct sigaction *old_sa);

int		check_signal_callback(void);

void 	redirections(t_shell_data *data, t_cmd *cmd);

char	**convert_list_to_tab_str(t_env *env);

int		ft_envsize(t_env *lst);

char    *shell_quote_removal(char *str);

/*============================== main_and_input ==============================*/

void	process_shell_input(char *input, t_shell_data *shell_data);

int	main(int ac, char **av, char **envp);

t_cmd	*parse_tokens(t_ta *ta);

void	ensure_stdin_tty(void);

int	has_eof_been_reached(void);

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
char			**allocate_env_array(t_env *env, int *t_tot);

int				fill_env_array(char **env_array, t_env *env);

t_env			*create_env_node(char *env_str);

void			add_env_node(t_env **env_list, t_env *new_node);

t_env			*init_env(char **envp);

/* shell_core.c */
t_shell_data	*init_shell_data(char **envp);

int				initialize_shell(t_shell_data **shell, char **envp);

void			update_shlvl(t_env **env, int level);

void			initialize_shlvl(t_env **env);

/*================================== syntax ==================================*/

int				shell_syntax_check(const char *line);

//lexer

t_ta			*clean_lexer(t_ta *ta);

t_ta			*new_lexer(char *input, t_shell_data *shell_data);
int				is_only_quotes(const char *input);
t_ta			*create_special_empty_token(t_ta *ta);
int				check_unclosed_quotes(t_ta *ta, t_shell_data *shell_data);
t_ta			*clean_lexer(t_ta *ta);

/*================================== parser ==================================*/
void			add_argument(t_cmd *cmd, char *arg, int quoted);

int				get_redirect_type(char *token);

void			add_redirect(t_cmd *cmd, int type, char *file, int eof_quoted);

void			cleanup_pipe_data(t_ta *new_ta, char **sub_tokens, int last_alloc);

t_ta			*init_new_ta(t_ta *ta, int index);

char			**create_sub_tokens(t_ta *ta, int index, t_ta *new_ta);

/* tokenarray_utils.c */

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

/*============================================================================*/

#endif


