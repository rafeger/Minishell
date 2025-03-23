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
	REDIR_IN = 3,
	REDIR_OUT = 4,
	REDIR_APP = 5,
	HEREDOC = 7,
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

char *readline(const char *prompt);

#endif

