/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fdf.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: labderra <labderra@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/24 21:20:46 by labderra          #+#    #+#             */
/*   Updated: 2024/08/02 00:23:54 by labderra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FDF_H
# define FDF_H
# include "MLX42/include/MLX42/MLX42.h"
# include "libft/libft.h"
# include <fcntl.h>
# include <math.h>

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
	uint32_t 	img_width;
	uint32_t	img_height;
	float 		scale;
	int			origin[2];
	t_point		**map_data;
}	t_map;


#endif