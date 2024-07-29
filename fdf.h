/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fdf.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: labderra <labderra@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/24 21:20:46 by labderra          #+#    #+#             */
/*   Updated: 2024/07/28 12:18:07 by labderra         ###   ########.fr       */
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
	uint32_t	x;
	uint32_t	y;
	uint32_t	c;
}	t_point;

typedef struct s_map
{
	int		max_x;
	int		max_y;
	int 	img_width;
	int		img_height;
	float 	scale_factor;
	t_point	**map_data;
}	t_map;


#endif