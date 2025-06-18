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

CFLAGS		=	-Wall -Werror -Wextra -I./include -I./libft

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
				syntax/check_path_core.c syntax/check_pipe_core.c \
				syntax/check_syntax_core.c syntax/check_quotes_core.c \
				lexer/lexer_core.c lexer/lexer_quote.c lexer/lexer_special.c \
				lexer/lexer_util.c \
				parser/parse_core.c parser/parse_args.c \
				parser/parse_redirect.c parser/tokenarray_utils.c \
				expand/expand_core.c expand/expand_size_utils.c \
				expand/expand_var_utils.c \
				execute/command_path.c execute/execute_external.c \
				execute/fork_and_execute.c execute/invalid_command_exp.c \
				execute/pipe_setup.c execute/execute_and_pipe_utils.c \
				execute/execute_external_utils.c execute/heredoc_core.c \
				execute/pipe_cleanup.c execute/pipe_utils.c \
				execute/execute_core.c execute/execute_single.c \
				execute/invalid_command_core.c execute/pipe_core.c \
				execute/redirections_core.c execute/execute_utils.c \
				execute/heredoc_sig.c execute/heredoc_utils.c \
				builtins/cd_builtin.c builtins/env_builtin.c \
				builtins/export_builtin.c builtins/export_builtin_utils.c \
				builtins/unset_builtin.c builtins/echo_builtin.c \
				builtins/exit_builtin.c builtins/export_builtin_error.c \
				builtins/pwd_builtin.c)

OBJS		=	$(SRCS:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

LIBFT		=	$(LIBFT_DIR)/libft.a

all:			$(NAME)

$(NAME):		$(LIBFT) $(OBJS)
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
