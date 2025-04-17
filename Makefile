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
		$(SRCDIR)/exec/utils.c

OBJ := $(SRC:.c=.o)



all: $(LIBFT) $(NAME)

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) $(LIBFT) -o $(NAME)

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

