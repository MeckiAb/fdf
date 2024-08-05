# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: labderra <labderra@student.42malaga.com    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/07/24 20:19:18 by labderra          #+#    #+#              #
#    Updated: 2024/08/04 18:09:34 by labderra         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = fdf
LIBFT = libft/libft.a
MLX42 = ./MLX42/build/libmlx42.a 
MLX_FLAGS = -Iinclude -ldl -lglfw -pthread -lm

SRC = main.c \
initialize.c \
plotting.c \
tools.c

CC = clang -Wall -Wextra -Werror -g -fsanitize=address

OBJ = $(SRC:.c=.o)

all : $(NAME)

$(NAME) : $(OBJ) $(LIBFT) $(MLX42)
	$(CC) $(OBJ) $(LIBFT) $(MLX42) $(MLX_FLAGS) -o $@

$(LIBFT) :
	make -C "libft"

$(MLX42) :
	cmake -B MLX42/build -S ./MLX42
	cmake --build MLX42/build -j4

%.o : %.c
	$(CC) -c $< -o $@

clean :
	rm -f $(OBJ)
	rm -f $(OBJ_BONUS)
	make -C "libft" clean

fclean : clean
	rm -f $(NAME)
	rm -f $(BONUS)
	make -C "libft" fclean

re : fclean all

.PHONY : all clean fclean re