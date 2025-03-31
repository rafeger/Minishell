# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: zmurie <marvin@42.fr>                      +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/11/06 16:29:49 by zmurie            #+#    #+#              #
#    Updated: 2024/11/25 11:11:23 by zmurie           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

#Nom de la librairie
NAME = libft.a

#Fichiers sources et objets
SRC = ft_atoi.c ft_bzero.c ft_calloc.c ft_isalnum.c ft_isalpha.c ft_isascii.c ft_isdigit.c ft_isprint.c ft_itoa.c ft_memchr.c ft_memcmp.c ft_memcpy.c ft_memmove.c ft_memset.c ft_putchar_fd.c ft_putendl_fd.c ft_putnbr_fd.c ft_putstr_fd.c ft_split.c ft_strchr.c ft_strdup.c ft_striteri.c ft_strjoin.c ft_strlcat.c ft_strlcpy.c ft_strlen.c ft_strmapi.c ft_strncmp.c ft_strnstr.c ft_strrchr.c ft_strtrim.c ft_substr.c ft_tolower.c ft_toupper.c ft_printf.c ft_putchar.c ft_puthex.c ft_putnbr.c ft_putpointer.c ft_putstr.c ft_putunsigned.c get_next_line_utils.c get_next_line.c ft_strndup.c
BSRC = ft_lstadd_back_bonus.c ft_lstadd_front_bonus.c ft_lstclear_bonus.c ft_lstdelone_bonus.c ft_lstiter_bonus.c ft_lstlast_bonus.c ft_lstmap_bonus.c ft_lstnew_bonus.c ft_lstsize_bonus.c
OBJ = $(SRC:.c=.o)
BOBJ = $(BSRC:.c=.o)

#Options de compilation
CFLAGS = -Wall -Wextra -Werror

#Regle paer defaut (all)
all: $(NAME)

#Compilatin de la librairie
$(NAME) : $(OBJ)
	ar rcs $(NAME) $(OBJ)

# Compilation des fichiers bonus
bonus: $(OBJ) $(BOBJ)
	ar rcs $(NAME) $(BOBJ)

#Compilation des fichiers .c en .o
%.o: %.c
	gcc $(CFLAGS) -c $< -o $@

# Nettoyage des fichiers objets
clean:
	rm -f $(OBJ) $(BOBJ)
	
#Nettoyage complet (fichiers objets + librairie)
fclean: clean
	rm -f $(NAME)

#Re-compilation complete
re: fclean all
