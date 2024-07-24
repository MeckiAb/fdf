# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: labderra <labderra@student.42malaga.com    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/07/24 20:19:18 by labderra          #+#    #+#              #
#    Updated: 2024/07/24 20:57:55 by labderra         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = fdf
#BONUS = fdf
LIBFT = libft/libft.a
FT_PRINTF = ft_printf/libftprintf.a
# MATH = -lm
MLX42 = ./MLX42/build/libmlx42.a 
MLX_FLAGS = -I ./MLX42/include -ldl -lglfw -pthread -lm

SRC = main.c 

SRC_BONUS = main_bonus.c 

CC = clang -Wall -Wextra -Werror -g -fsanitize=address

OBJ = $(SRC:.c=.o)
OBJ_BONUS = $(SRC_BONUS:.c=.o)

all : $(NAME)

$(NAME) : $(OBJ) $(LIBFT) $(FT_PRINTF) $(MLX42)
	$(CC) $(OBJ) $(LIBFT) $(FT_PRINTF) $(MLX42) $(MLX_FLAGS) -o $@

$(LIBFT) :
	make -C "libft"
#	make -C "libft" bonus

$(FT_PRINTF) :
	make -C "ft_printf"

$(MLX42) :
	cd MLX42
	cmake -B build
	cmake --build build -j4
	cd ..

%.o : %.c
	$(CC) -c $< -o $@

clean :
	rm -f $(OBJ)
	rm -f $(OBJ_BONUS)
	make -C "libft" clean
	make -C "ft_printf" clean

fclean : clean
	rm -f $(NAME)
	rm -f $(BONUS)
	make -C "libft" fclean
	make -C "ft_printf" fclean

re : fclean all

# bonus: $(BONUS)

# $(BONUS) : $(OBJ_BONUS) $(LIBFT) $(FT_PRINTF)
# 	$(CC) $(OBJ_BONUS) $(LIBFT) $(FT_PRINTF) -o $@

.PHONY : all clean fclean re # bonus