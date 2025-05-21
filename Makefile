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
NAME := minishell

LIBS = -lreadline
CC := cc
CFLAGS := -Wall -Wextra -Werror -g -Iheaders/

# Bibliothèque Libft
LIBFT := ./libft/libft.a

# Liste des fichiers sources
SRCDIR := src

SRC :=  $(SRCDIR)/builtin/ft_cd.c			\
        $(SRCDIR)/builtin/ft_echo.c 		\
        $(SRCDIR)/builtin/ft_env.c			\
        $(SRCDIR)/builtin/ft_exit.c			\
        $(SRCDIR)/builtin/ft_export.c		\
        $(SRCDIR)/builtin/ft_export2.c		\
        $(SRCDIR)/builtin/ft_pwd.c			\
		$(SRCDIR)/builtin/ft_unset.c		\
		$(SRCDIR)/exec/builtin.c			\
		$(SRCDIR)/exec/exec.c				\
		$(SRCDIR)/exec/find_cmd.c			\
		$(SRCDIR)/exec/here_doc.c			\
		$(SRCDIR)/exec/utils.c				\
		$(SRCDIR)/parsing/append_args.c		\
		$(SRCDIR)/parsing/create_cmd.c		\
		$(SRCDIR)/parsing/extract_token.c	\
		$(SRCDIR)/parsing/expand_vars.c 	\
		$(SRCDIR)/parsing/tokenizer_utils.c	\
		$(SRCDIR)/parsing/tokenizer.c		\
		$(SRCDIR)/trucs/free.c				\
		$(SRCDIR)/trucs/misc_tokens.c       \
		$(SRCDIR)/trucs/debug.c				\
		$(SRCDIR)/main.c

OBJ := $(SRC:.c=.o)



all: $(LIBFT) $(NAME)

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) $(LIBFT) -o $(NAME) $(LIBS)

# Règle pour compiler Libft
$(LIBFT):
	make -C libft

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	make clean -C libft
	rm -rf $(OBJ)

fclean: clean
	make fclean -C libft
	rm -rf $(NAME)

re: fclean all

.PHONY: all clean fclean re

