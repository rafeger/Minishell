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

typedef struct s_env
{
	char				*key;
	char				*value;
	struct s_env		*next;
	struct s_env		*prev;
}	t_env;

typedef struct s_ta
{
	char				**tokens;
	char				*token;
	bool				in_q;
	int					t_tot;
	int					cap;
	int					trailing_space;
	int					*quoted;
	int					second_quote;
	char				quotechar;
	size_t				tokenindex;
	size_t				tokensize;
}	t_ta;

// typedef enum e_redir
// {
// 	REDIR_IN,
// 	REDIR_OUT,
// 	HERE_DOC,
// 	APPEND
// }	e_t_redir;

typedef struct s_redirect
{
	int					type;
	int					eof_quoted;
	char				*file;
	struct s_redirect	*next;
}	t_redirect;

typedef struct s_cmd
{
	int					arg_count;
	char				**args;
	char				*c_name;
	t_redirect			*redirects;
	struct s_cmd		*next;
	struct s_cmd		*prev;
	t_fd_info			*fd_info;
	pid_t				pid;
	int					tty_backup;
	int					heredoc_fd;
	int					quoted;
	int					*arg_quoted;
	bool				heredoc_quotes;
}	t_cmd;

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

void	clean_spaces_loop(const char *str, char *result, int *i, int *j);

char	*clean_spaces(const char *str);

/*============================== exec_zoe ====================================*/
int		is_builtin(char *cmd);

int		do_builtin(t_shell_data *data, t_cmd *cmd);

int		execute_commands(t_shell_data *data);

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

int		print_error_opening_file(char *name_file);

char	*generate_random_filename(void);

/*============================== main_and_input ==============================*/

void	process_shell_input(char *input, t_shell_data *shell_data);
int	main(int ac, char **av, char **envp);
t_cmd	*parse_tokens(t_ta *t_array);
void	ensure_stdin_tty(void);
int	has_eof_been_reached(void);

/*=================================== env ====================================*/


char	*find_env_val(t_env *env_head, const char *k);
void	set_env_variable(t_env **env_head, const char *k, const char *v);
void	delete_env_entry(t_env **env_head, const char *k);
char	**envlist_to_strarr(t_env *env_head);
char 	*make_env_kv_string(const char *k, const char *v);

t_env	*build_env_list(char **envp);
void	append_env_entry(t_env **env_head, t_env *new_entry);
char	**alloc_env_strs(t_env *env_head, int *env_count);
int		populate_env_strs(char **strarr, t_env *env_head);
t_env	*parse_env_pair(char *env_str);

int		setup_shell(t_shell_data **sh, char **envp);

void	refresh_exit_status(t_shell_data *shdata);
void	setup_signals(void);

/*================================== syntax ==================================*/

int				shell_syntax_check(const char *line);
t_ta			*clean_lexer(t_ta *t_array);
t_ta			*lexer(char *input, t_shell_data *shell_data);
int				is_only_quotes(const char *input);
t_ta			*create_special_empty_token(t_ta *t_array);
int				check_unclosed_quotes(t_ta *t_array, t_shell_data *shell_data);
t_ta			*clean_lexer(t_ta *t_array);

/*================================== parser ==================================*/
void			add_argument(t_cmd *cmd, char *arg, int quoted);
int				get_redirect_type(char *token);
void			add_redirect(t_cmd *cmd, int type, char *file, int eof_quoted);
void			cleanup_pipe_data(t_ta *new_ta, char **sub_tokens, int last_alloc);
t_ta			*init_new_ta(t_ta *t_array, int index);
char			**create_sub_tokens(t_ta *t_array, int index, t_ta *new_ta);
int				init_quoted_array(t_ta *new_ta, t_ta *t_array, int index);

/*================================== expand ==================================*/

// ===================== expand_core.c =====================
int		get_var_length(const char *str);
char	*get_var_value(const char *var, t_shell_data *shell_data);
void	process_expand_char(t_exp *exp, char *input);
size_t	calculate_expanded_size(char *input, t_shell_data *shell_data);
char	*expand_variables(char *input, t_shell_data *shell_data);

// ===================== expand_utils.c =====================
int		handle_quoted_len(char **result, int *j, char *input, int quote_len);
int		process_var(char *input, int i, size_t *size, t_shell_data *sd);
int		is_in_quotes(const char *str);
void	copy_var_value(char **result, int *j, char *var_value);
int		get_quoted_length(const char *str);

// ===================== expand_quoted.c =====================
int		handle_quoted_var(char **result, int *j, char *input);

// ===================== expand_handle_var.c =====================
int		handle_var(char **res, int *j, char *in, t_shell_data *sd);

/*=============================== init_and_free ==============================*/

/* free_core.c */
void			free_ptr(void *ptr);
void			free_env_array(char **env_array);
void			free_tokenarray(t_ta *t_array);
void			free_redirects(t_redirect *redirect);

/* tokenarray_init.c */
void			tokenarray_init_second(t_ta *t_array);
t_ta			*tokenarray_init(void);

/* struct_init_core.c */
t_cmd			*cmd_initialisation(void);
void			fd_info_init(t_cmd *cmd);

/* free_advanced.c */
void			free_command_args(t_cmd *cmd);
void			free_command(t_cmd *cmd);
void			ft_cleanup_shell(t_shell_data **shell);
void			ft_cleanup_env(t_env **env);

/*============================================================================*/
// lexer_core.c
int		is_quote_char(char c);
int		is_special_char(char c);
void	ensure_token_capacity(t_ta *lx);
void	expand_token_array(t_ta *lx);

// lexer_token.c
int		add_token_failed_new(t_ta *lx);
int		add_token_new(t_ta *lx, char *token);
void	append_trailing_space_new(t_ta *lx, int was_quoted);
void	finalize_token_new(t_ta *lx);

// lexer_quotes.c
void	quote_state_machine_new(t_ta *lx, char c);
void	handle_quotes_new(t_ta *lx, char **input);

// lexer_special.c
void	handle_special_token_new(t_ta *lx, char **input);
void	handle_special_chars_new(t_ta *lx, char **input);

// lexer_driver.c
void	lex_step_new(t_ta *lx, char **input);
void	lex_input_new(t_ta *lx, char *input);
t_ta	*lexer(char *input, t_shell_data *shell_data);
int		is_only_quotes(const char *input);
t_ta	*create_special_empty_token(t_ta *t_array);
int		check_unclosed_quotes(t_ta *t_array, t_shell_data *shell_data);
t_ta	*clean_lexer(t_ta *t_array);
#endif


