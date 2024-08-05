/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   plotting.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: labderra <labderra@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/04 17:18:52 by labderra          #+#    #+#             */
/*   Updated: 2024/08/04 17:34:34 by labderra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

static uint32_t	grad_color(uint32_t start, uint32_t end, float step)
{
	int			j;
	uint32_t	color;
	uint8_t		byte_start;
	uint8_t		byte_end;

	color = 0;
	j = 24;
	while (j >= 0)
	{
		byte_start = start >> j;
		byte_end = end >> j;
		color = color * 256;
		if (byte_start > byte_end)
			byte_end = byte_start - (byte_start - byte_end) * step;
		else
			byte_end = byte_start + (byte_end - byte_start) * step;
		color = color + byte_end;
		j -= 8;
	}
	return (color);
}

static void	swap_points(t_point *start, t_point *end)
{
	t_point	swap;

	swap = *start;
	*start = *end;
	*end = swap;
}

static void	plot_line(t_point start, t_point end, mlx_image_t *img, t_map *fdf)
{
	float	m;
	int		b;
	int		i;

	m = (float)(end.y - start.y) / (float)(end.x - start.x);
	if ((m <= 1.0 && m >= -1.0 && start.x > end.x)
		|| ((m > 1.0 || m < -1.0) && start.y > end.y))
		swap_points(&start, &end);
	b = (start.y - m * start.x) * fdf->scale;
	if (m <= 1.0 && m >= -1.0)
	{
		i = start.x * fdf->scale;
		while (i++ < end.x * fdf->scale)
			mlx_put_pixel(img, i + fdf->o_xy[0], i * m + fdf->o_xy[1] + b,
				grad_color(start.c, end.c, (i - start.x * fdf->scale)
					/ ((end.x - start.x) * fdf->scale)));
		return ;
	}
	i = start.y * fdf->scale;
	while (i++ < end.y * fdf->scale)
		mlx_put_pixel(img, fdf->o_xy[0] + (i - b) / m, i + fdf->o_xy[1],
			grad_color(start.c, end.c, (i - start.y * fdf->scale)
				/ ((end.y - start.y) * fdf->scale)));
}

void	plot_map(t_map *fdf, mlx_image_t *img)
{
	int	i;
	int	j;

	i = 0;
	while (i < fdf->max_x)
	{
		j = 0;
		while (j < fdf->max_y)
		{
			if (i < fdf->max_x - 1)
				plot_line(fdf->map_data[i][j], fdf->map_data[i + 1][j],
					img, fdf);
			if (j < fdf->max_y - 1)
				plot_line(fdf->map_data[i][j], fdf->map_data[i][j + 1],
					img, fdf);
			j++;
		}
		i++;
	}
}
