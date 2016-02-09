# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: udelorme <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2016/01/14 11:43:18 by udelorme          #+#    #+#              #
#    Updated: 2016/02/09 10:47:37 by udelorme         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC = clang
FLAGS = -Wall -Werror -Wextra
NAME = fdf
SRC = main.c 
OBJ = $(SRC:.c=.o)

SRCDIR = src/
OBJDIR = obj/
INCDIR = includes/
LIBDIR = libft/
MLXDIR = minilibx_macos

all: $(NAME)

$(NAME):
	(cd $(LIBDIR); $(MAKE) all)
	(cd $(MLXDIR); $(MAKE) all)
	(cd $(SRCDIR);$(CC) $(FLAGS) -c $(SRC) -I ../$(INCDIR) -I ../$(LIBDIR)includes/;mv $(OBJ) ../obj)
	(cd $(OBJDIR);$(CC) $(FLAGS) -o ../$(NAME) $(OBJ) -L ../$(LIBDIR) -L ../$(MLXDIR) -lft -lmlx -framework OpenGL -framework AppKit)

clean:
	(cd $(OBJDIR); rm -rf $(OBJ))

fclean: clean
	rm -rf $(NAME)

re: fclean all

lftclean:
	(cd $(LIBDIR); $(MAKE) clean)

lftfclean:
	(cd $(LIBDIR); $(MAKE) fclean)

mlxclean:
	(cd $(MLXDIR); $(MAKE) clean)

.PHONY: $(NAME) all clean fclean re mlxclean lftclean lftfclean
