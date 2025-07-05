# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: zmurie <marvin@42.fr>                      +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/04/17 11:10:47 by zmurie            #+#    #+#              #
#    Updated: 2025/04/17 11:10:49 by zmurie           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #
NAME		=	minishell

CC			=	cc

CFLAGS		=	-g3 -Wall -Werror -Wextra -I./include -I./libft

HEADER		=	include/minishell.h

SRC_DIR		=	src

OBJ_DIR		=	obj

LIBFT_DIR	=	libft

SRCS		=	$(addprefix $(SRC_DIR)/, \
				main/main.c main/readline_core.c main/main_utils.c \
				init_and_free/free_all.c init_and_free/free_all2.c \
				init_and_free/tokens_cmd_init.c init_and_free/free_all3.c \
				env_vars/env_core.c env_vars/env_init.c env_vars/shell_core.c \
				env_vars/signal_core.c \
				check_syntax/check_syntax_main.c check_syntax/pipe_errors.c \
				check_syntax/redir_double.c check_syntax/redir_errors.c \
				check_syntax/syntax_utils.c \
				tok/append_token.c tok/lex_core.c tok/lexer_utils2.c \
				tok/quote_handle.c tok/spe_char_handle.c \
				parser/parse_core.c parser/parse_args.c \
				parser/parser_utils.c \
				parser/parse_redirect.c tok/tokenarray_utils.c \
				exp/expand_dollar.c exp/expand_main.c exp/expand_size.c \
				exp/expand_utils.c exp/expand_variables.c \
				exec_zoe/builtin.c \
				exec_zoe/redir_no_pipe.c \
				exec_zoe/exec.c exec_zoe/exec_pipes.c exec_zoe/exec_process.c exec_zoe/exec_utils.c \
				exec_zoe/find_cmd.c exec_zoe/here_doc.c exec_zoe/here_doc2.c \
				exec_zoe/redirections.c exec_zoe/utils.c \
				exec_zoe/random_name_file.c \
				builtin_zoe/ft_cd.c builtin_zoe/ft_echo.c \
				builtin_zoe/ft_env.c builtin_zoe/ft_exit.c \
				builtin_zoe/ft_export.c builtin_zoe/ft_export2.c \
				builtin_zoe/ft_export3.c builtin_zoe/ft_export4.c\
				builtin_zoe/ft_pwd.c builtin_zoe/ft_unset.c)

OBJS		=	$(SRCS:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

LIBFT		=	$(LIBFT_DIR)/libft.a

all:			$(NAME)

$(NAME):		$(LIBFT) $(OBJS) $(HEADER)
	$(CC) $(CFLAGS) $(OBJS) -L$(LIBFT_DIR) -lft -lreadline -o $(NAME)

$(OBJ_DIR)/%.o:	$(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(LIBFT):
	make -C $(LIBFT_DIR)

clean:
	rm -rf $(OBJ_DIR)
	make -C $(LIBFT_DIR) clean

fclean: clean
	rm -f $(NAME)
	make -C $(LIBFT_DIR) fclean

re:				fclean all

.PHONY:			all clean fclean re
