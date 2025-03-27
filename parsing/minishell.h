#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <stdlib.h>
# include <readline/readline.h>
# include <readline/history.h>

typedef enum	e_token_ID
{
	WORD = 1,
	PIPE = 2,
	REDIR_IN = 3, //<
	REDIR_OUT = 4, //>
	REDIR_APP = 5, //<<
	
	HEREDOC = 6,
	END = 7
}	t_token_type;


typedef	struct s_token
{
	t_token_type type;
	char		*value;
	struct s_token *next;
}	t_token;


typedef struct s_command
{
	char	**args;
	char	*infile;
	char	*outfile;
	int		append;
	int		pipe;
	struct s_command *next;
}	t_command;

/* la struct commant en liste chainnee pratique parceque on 
peut avoir nos differents args avec les pipes | dans une struct chacune
pas de conflits d'args

un infile et un outfile pour les > et <

ex :  si on a genre cat < input.txt > output.txt

on peut stocker ca de maniere :

cmd.args = ["cat", NULL];
cmd.infile = "input.txt";
cmd.outfile = "output.txt";
cmd.append = 0;

(append pour differencier si on ecrase dans le fichier sortie ou pas);*/

//utils
char	*ft_strndup(char *input, int start, int end);
void	syntax_error(char error);
void	*ft_memset(void *s, int c, size_t n);

//handle_quotes
void	handle_single_quotes(t_token *tokens, char *input, int *i);
void	handle_double_quotes(t_token *tokens, char *input, int *i);


//tokenizator
void	add_token(t_token **tokens, t_token_type type, char *value);
int		extract_word(t_token **tokens, char *input, int i);
t_token	*lexer(char	*input);

//tokens_class
void	handle_quotes(t_token *tokens, char *input, int *i, char quote_type);
void	handle_whitespace(char *input, int *i);
void 	handle_pipe(t_token *tokens, char *input, int *i);
void	handle_redir(t_token *tokens, char *input, int *i);


#endif

