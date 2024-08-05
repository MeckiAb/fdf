/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fdf.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: labderra <labderra@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/24 21:20:46 by labderra          #+#    #+#             */
/*   Updated: 2024/08/04 17:31:40 by labderra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FDF_H
# define FDF_H
# include "MLX42/include/MLX42/MLX42.h"
# include "libft/libft.h"
# include <fcntl.h>
# include <stdio.h>

typedef struct s_point
{
	int			x;
	int			y;
	uint32_t	c;
}	t_point;

typedef struct s_map
{
	char		*filename;
	int			max_x;
	int			max_y;
	int			x_rng[2];
	int			y_rng[2];
	uint32_t	img_width;
	uint32_t	img_height;
	float		scale;
	int			o_xy[2];
	t_point		**map_data;
}	t_map;

int			init_fdf(t_map **fdf, char *filename);
int			load_map(t_map *fdf);
void		plot_map(t_map *fdf, mlx_image_t *img);
uint32_t	load_color(char *str);
uint32_t	atoi_x(char *str);
void		free_map(t_map *fdf);
void		free_matrix(char ***matrix);

#endif