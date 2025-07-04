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
				main_and_input/main.c main_and_input/main_utils.c \
				main_and_input/input_core.c \
				init_and_free/struct_init_core.c init_and_free/free_core.c \
				init_and_free/tokenarray_init.c init_and_free/free_advanced.c \
				env/env_core.c env/env_init.c env/signal_core.c \
				env/shell_core.c \
				syntax_new/check_syntax_core_new.c \
				lexer/lexer_core.c lexer/lexer_quote.c lexer/lexer_special.c \
				lexer/lexer_token.c lexer/lexer_utils.c \
				parser/parse_core.c parser/parse_args.c \
				parser/parse_redirect.c parser/tokenarray_utils.c \
				expand/expand_core.c expand/expand_handle_var.c \
				expand/expand_quoted.c expand/expand_utils.c \
				exec_zoe/builtin.c \
				exec_zoe/redir_no_pipe.c \
				exec_zoe/exec.c exec_zoe/exec_pipes.c exec_zoe/exec_process.c exec_zoe/exec_utils.c \
				exec_zoe/find_cmd.c exec_zoe/here_doc.c exec_zoe/here_doc2.c \
				exec_zoe/redirections.c exec_zoe/utils.c \
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

fclean:			clean
	rm -f $(NAME)
	make -C $(LIBFT_DIR) fclean

re:				fclean all

.PHONY:			all clean fclean re
