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



// typedef struct s_command
// {
// 	char				**args;
// 	int					infile;
// 	int					outfile;
// 	int					append;
// 	char 				*heredoc_delim;
// 	struct s_command	*next;
// }	t_command;

// typedef struct s_env
// {
// 	char			*str;
// 	struct s_env	*prev;
// 	struct s_env	*next;
// }					t_env;


// extern pid_t	g_signal;

// int		ft_cd(char **args, t_env *env);
// int		ft_echo(char **arg);
// int		ft_env(t_env *env);
// void	ft_exit(t_shell_data *data, char **args);
// int		check_valid_name_var(char *str);
// int		ft_export(char **args, t_env **env);
// int		ft_pwd(void);
// int		ft_unset(char **args, t_env **env);
// int		is_builtin(char *cmd);
// void	do_builtin(t_shell_data *data, t_cmd *cmd);
// char	*get_pathname(char *cmd, t_env *env);
// char	**convert_list_to_tab_str(t_env *env);
// void	free_all(t_shell_data *data);
// int		ft_envsize(t_env *lst);

/*============================== main_and_input ==============================*/

void	process_shell_input(char *input, t_shell_data *shell_data);

int	main(int ac, char **av, char **envp);


void	ensure_stdin_tty(void);

int	has_eof_been_reached(void);

// /* main.c */
// char			*get_input(void);

// int				main(int ac, char **av, char **envp);

// /* input_core.c */
// void			cleanup_current_cmd(t_shell_data *shell_data);

// int				handle_syntax_check(char *input, t_shell_data *shell_data);

// t_cmd			*execute_input(char *input, t_shell_data *shell_data);

// void			execute_if_valid(t_cmd *cmd, t_shell_data *sd);

// void			handle_input(char *input, t_shell_data *shel_data);

// /* main_utils.c */

// void			check_and_restore_stdin(void);

// int				is_eof_reached(void);

/*================================= execute ==================================*/

/* command_path.c */

char			*find_command_path(const char *cmd, t_env *env);

/* execute_and_pipe_utils.c */
int				is_pipeline(t_cmd *cmd);

int				is_builtin(char *cmd_name);

/* execute_utils.c */
int				handle_special_cases(t_cmd *cmd, t_shell_data *shell_data);

/* execute_core.c */
void			execute_commands(t_cmd *cmd, t_shell_data *shell_data);

/* execute_single.c */
int				execute_builtin(t_cmd *cmd, t_shell_data *shell_data);

void			execute_child_process(t_cmd *cmd, t_shell_data *shell_data);

/* pipe_core.c */
void			execute_pipeline(t_cmd *cmd, t_shell_data *shell_data);

/* pipe_setup.c */

int				handle_pipe(t_cmd *cmd, t_ta *ta, int index);

void			create_all_pipes(t_cmd *cmd_list);

/* pipe_cleanup.c */
void			cleanup_pipeline_fds(t_cmd *cmd);

void			cleanup_command_fds(t_cmd *cmd);

void			cleanup_pipe(t_fd_info *fd_info);

void			cleanup_all_pipes(t_cmd *cmd);

void			cleanup_heredocs(t_cmd *cmd);

/* pipe_utils.c */
void			backup_fds(t_fd_info *fd_info);

void			restore_fds(t_fd_info *fd_info, t_cmd *cmd);

/* redirections_core.c */
int				apply_heredocs_only(t_redirect *redirects, t_shell_data *sd);

int				apply_other_redirs(t_redirect *redirects, t_cmd *cmd);

/* fork_and_execute.c */

int				fork_and_execute(t_cmd *cmd, t_shell_data *sd, \
					int input_fd, int *pipe_fd);

/* invalid_command_core.c */

int				handle_invalid_command(t_cmd *cmd, t_shell_data *shell_data);

/* invalid_command_expanded.c */
int				is_expanded_invalid_cmd(const char *name, int quoted, \
					t_shell_data *sd);

void			handle_expanded_invalid_cmd(t_cmd *cmd, t_shell_data *sd);

/* execute_external.c */

void			execute_external(t_cmd *cmd, t_shell_data *shell_data);

void			handle_external_command(t_cmd *cmd, t_shell_data *shell_data);

/* execute_external_utils.c */
void			check_file_permissions(char *path, t_cmd *cmd, \
										t_shell_data *sd);

void			handle_command_error(t_cmd *cmd, t_shell_data *sd, \
										int is_permission);

int				check_redirect_arg_error(char **args, t_shell_data *shell_data);

/* heredoc_core.c */
int				handle_heredoc(char *delimiter, int eof_quoted, \
					t_shell_data *sd);

/* heredoc_sig.c */
int				setup_signal_handlers(struct sigaction *sa_new, struct \
					sigaction *sa_old);

int				handle_heredoc_input(t_heredoc_data *her_data, \
					struct sigaction *sa_old);

/* heredoc_utils.c */
int				process_heredoc(t_redirect *current, t_shell_data *sd);

int				handle_single_heredoc(t_cmd *current, t_redirect *redir, \
					t_shell_data *sd);

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

/*================================= builtins =================================*/

/* cd_builtin.c */
int				builtin_cd(t_cmd *cmd, t_shell_data *shell_data);

/* echo_builtin.c */
int				builtin_echo(t_cmd *cmd);

/* env_builtin.c */
void			builtin_env(t_shell_data *shell_data);

/* exit_builtin.c */
int				builtin_exit(t_cmd *cmd, t_shell_data *shell_data);

/* export_builtin.c */
int				builtin_export(t_cmd *cmd, t_shell_data *shell_data);

/* export_builtin_utils.c */
void			export_error(char *identifier, char *arg, t_shell_data *sd);

int				check_exclamation_export(const char *str);

int				is_valid_first_char(char c);

int				is_valid_identifier_char(char c);

int				is_valid_identifier(const char *str);

/* export_builtin_error.c */
void			export_event_error(char *full_arg);

/* unset_builtin.c */
void			builtin_unset(t_cmd *cmd, t_shell_data *shell_data);

/* pwd_builtin.c */
int				builtin_pwd(t_cmd *cmd);

/*================================== syntax ==================================*/

/* check_pipe_core.c */
int				check_redir_pipe(char *input, int *i);

int				check_pipe_sequence(char *input, int *i);

int				check_pipe_start(char *input);

int				check_pipe_syntax(char *input);

/* check_path_core.c */
int				is_dot_command(const char *cmd);

int				is_only_path_chars(char *input);

void			handle_dot_command(const char *cmd, t_shell_data *shell_data);

int				check_directory_path(char *input);

/* check_syntax_core.c */
int				check_multi_redir(char *input, int *i);

int				check_newline_after_redir(char *input, int i);

int				check_consecutive_redir(char *input, int *i);

int				check_redirections(char *input, int *i);

int				check_syntax(char *input);

/* check_quotes_core.c */
int				check_if_quotes(char *input, int *i);

/*================================== lexer ===================================*/

// void	handle_char_tokenization(t_ta *ta, char **input);
// void	process_input(t_ta *ta, char **input);
// void	handle_token_end(t_ta *ta);
// int	append_token_to_array(t_ta *ta, char *token);
// t_ta	*tokenize_input_string(char *input);

// // From lexer_quote.c
// int	contains_only_quotes(const char *input);
// t_ta	*create_empty_quoted_token(t_ta *ta);
// void	process_empty_quotes(t_ta *ta, char **input);
// void	manage_quote_processing(t_ta *ta, char **input);
// void	process_quote_character(t_ta *ta, char *input);

// // From lexer_special.c
// void	handle_trailing_space(t_ta *ta, int was_quoted);
// void	handle_special_chars(t_ta *ta, char **input);
// void	resize_token_array_capacity(t_ta *ta);

// // From lexer_util.c
// int	handle_token_add_failure(t_ta *ta);
// int	validate_quote_closure(t_ta *ta);
// t_ta	*cleanup_lexer_resources(t_ta *ta);




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

/*============================================================================*/

#endif


