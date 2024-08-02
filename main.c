/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: labderra <labderra@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/24 20:32:49 by labderra          #+#    #+#             */
/*   Updated: 2024/08/03 01:04:55 by labderra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"
#include <stdio.h>

uint32_t	grad_color(uint32_t start, uint32_t end, float step)
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

void	swap_points(t_point *start, t_point *end)
{
	t_point	swap;

	swap = *start;
	*start = *end;
	*end = swap;
}
void	plot(t_point start, t_point end, mlx_image_t *img, t_map *fdf)
{
	float	m;
	int		b;
	int		i;

	m = (float)(end.y - start.y) / (float)(end.x - start.x);
	if ((m <= 1.0 && m >= -1.0 && start.x > end.x)
			|| ((m > 1.0 || m < -1.0) && start.y > end.y))
		swap_points(&start, &end);
	b = (start.y - m * start.x) * fdf->scale;
	if ( m <= 1.0 && m >= -1.0)
	{
		i = start.x * fdf->scale;
		while (i++ < end.x * fdf->scale)
			mlx_put_pixel(img, i + fdf->origin[0], i * m + fdf->origin[1] + b,
				grad_color(start.c, end.c,
				(i - start.x * fdf->scale) / ((end.x - start.x) * fdf->scale)));
		return ;
	}
	i = start.y * fdf->scale;
	while (i++ < end.y * fdf->scale)
		mlx_put_pixel(img, fdf->origin[0] + (i - b) / m, i + fdf->origin[1],
			grad_color(start.c, end.c, 
			(i - start.y * fdf->scale) / ((end.y - start.y) * fdf->scale)));
}

void	key_func(mlx_key_data_t keydata, void *param)
{
	if (keydata.key == MLX_KEY_ESCAPE && keydata.action == MLX_PRESS)
	{
		mlx_close_window((mlx_t *)param);
	}
}

void	free_matrix(char ***matrix)
{
	int	i;
	int	j;

	i = 0;
	while (matrix && matrix[i])
	{
		j = 0;
		while (matrix[i] && matrix[i][j])
			free(matrix[i][j++]);
		free(matrix[i++]);
	}
	free(matrix);
}

void	free_map(t_map *fdf)
{
	int	i;

	i = fdf->max_x;
	while (i--)
		free(fdf->map_data[i]);
	free(fdf->map_data);
}

uint32_t	atoi_x(char *str)
{
	uint32_t	acc;

	acc = 0;
	while ((*str >= '0' && *str <= '9') || (*str >= 'a' && *str <= 'f'))
	{
		if (*str < 'a')
			acc = acc * 16 + *str - 48;
		else
			acc = acc * 16 + *str - 'a' + 10;
		str++;
	}
	return (acc);
}

uint32_t	load_color(char *str)
{
	char		**tmp_color;
	uint32_t	color;
	int	i;

	color = 0xffffffff;
	if (ft_strchr(str, 'x'))
	{
		tmp_color = ft_split(str, 'x');
		if (!tmp_color || !tmp_color[1])
			perror("color parse fail");
		else
			color = atoi_x(tmp_color[1]) * 256 + 0xff;
		i = 0;
		while (tmp_color && tmp_color[i])
			free(tmp_color[i++]);
		free(tmp_color);
	}
	return (color);
}

void	set_origin(t_map *fdf)
{
	int	i;
	int j;
	
	fdf->x_rng[0] = fdf->map_data[0][fdf->max_y - 1].x;
	fdf->x_rng[1] = fdf->map_data[fdf->max_x - 1][0].x;
	i = fdf->max_x;
	while (i--)
	{
		j = fdf->max_y;
		while (j--)
		{
			if (fdf->map_data[i][j].y < fdf->y_rng[0])
				fdf->y_rng[0] = fdf->map_data[i][j].y;
			if (fdf->map_data[i][j].y > fdf->y_rng[1])
				fdf->y_rng[1] = fdf->map_data[i][j].y;
		}
	}
	fdf->scale = (fdf->img_width - 10.0) / (fdf->x_rng[1] - fdf->x_rng[0]);
	if ((fdf->img_height - 10.0) / (fdf->y_rng[1] - fdf->y_rng[0]) < fdf->scale)
		fdf->scale = (fdf->img_height - 10.0) / (fdf->y_rng[1] - fdf->y_rng[0]);
	fdf->origin[0] = fdf->img_width - fdf->x_rng[1] * fdf->scale - 5;
	fdf->origin[1] = fdf->img_height / 2 + (fdf->y_rng[0] - fdf->y_rng[1])
		* fdf->scale / 2 - 5;
}

char	***read_fdf(t_map *fdf)
{
	int		fd;
	char	*tmp_line;
	char	***tmp_map;
	int		i;

	tmp_map = (char ***)ft_calloc(1024, sizeof(char **));
	fd = open(fdf->filename, O_RDONLY);
	if (!tmp_map || fd == -1)
		return (NULL);
	tmp_line = get_next_line(fd);
	while (tmp_line)
	{
		tmp_map[fdf->max_y++] = ft_split(tmp_line, ' ');
		free(tmp_line);
		tmp_line = get_next_line(fd);
	}
	close (fd);
	while (tmp_map[0] && tmp_map[0][fdf->max_x])
		fdf->max_x++;
	i = fdf->max_y;
	while (i--)
		if (!tmp_map[i]
				|| !tmp_map[i][fdf->max_x - 1] || tmp_map[i][fdf->max_x])
			return (free_matrix(tmp_map), NULL);
	return (tmp_map);
}

int	load_map(t_map *fdf)
{
	char	***tmp_map;
	int		j;
	int		i;

	tmp_map = read_fdf(fdf);
	fdf->map_data = ft_calloc(sizeof(t_point *), fdf->max_x);
	if (!tmp_map || !fdf->map_data)
		return (free_matrix(tmp_map), free_map(fdf), -1);
	i = fdf->max_x;
	while (i--)
	{
		fdf->map_data[i] = ft_calloc(sizeof(t_point), fdf->max_y);
		if (!fdf->map_data[i])
			return (free_map(fdf), free_matrix(tmp_map), -1);
		j = fdf->max_y;
		while (j--)
		{
			fdf->map_data[i][j].x = (i - j) * 87;
			fdf->map_data[i][j].y = (i + j) * 50 - 100 * ft_atoi(tmp_map[j][i]);
			fdf->map_data[i][j].c = load_color(tmp_map[j][i]);
		}
	}
	set_origin(fdf);
	return (free_matrix(tmp_map), 0);		
}

int	init_fdf(t_map **fdf, char *filename)
{
	*fdf = malloc(sizeof(t_map));
	if ( !(*fdf))
		return (-1);
	(*fdf)->filename = filename;
	(*fdf)->max_x = 0;
	(*fdf)->max_y = 0;
	(*fdf)->x_rng[0] = 0;
	(*fdf)->x_rng[1] = 0;
	(*fdf)->y_rng[0] = 0;
	(*fdf)->y_rng[1] = 0;
	(*fdf)->img_width = 1000;
	(*fdf)->img_height = 1000;
	(*fdf)->scale = 0.01;
	(*fdf)->origin[0] = 0;
	(*fdf)->origin[1] = 0;
	(*fdf)->map_data = NULL;
	return (0);
}

int	main(int argc, char **argv)
{
	mlx_t		*mlx;
	mlx_image_t	*img;
	int			i;
	int			j;
	t_map		*fdf;
	
/* 	t_point		origin = {5, 5, 0x00ffffff};
	t_point		end = {350, 250, 0xff0000ff}; */

	if (argc != 2 || !ft_strnstr(argv[1], ".fdf\0", ft_strlen(argv[1]) + 1))
		return (write(2, "Usage : ./fdf <filename.fdf>\n", 29), 1);
	if (init_fdf(&fdf, argv[1]) == -1 || load_map(fdf) == -1)
		return (write(2, "Error reading map or Bad map\n", 29), 1);

	mlx = mlx_init(1000, 1000, "FdF", 1);
	if (!mlx)
		return (perror("mlx"), 1);
	img = mlx_new_image(mlx, fdf->img_width, fdf->img_height);

/* 	printf("x_range [%i, %i]\n", fdf->x_rng[0], fdf->x_rng[1]);
	printf("y_range [%i, %i]\n", fdf->y_rng[0], fdf->y_rng[1]);
	printf("origin (%i, %i)\n", fdf->origin[0], fdf->origin[1]);
	printf("scale %f\n", fdf->scale);
	printf("\n(%i, %i) -> (%i, %i)\n", fdf->map_data[0][0].x, fdf->map_data[0][0].y, 
		fdf->map_data[0][1].x, fdf->map_data[0][1].y);
 */		
	i = 0;
	while (i < fdf->max_x)
	{
		j = 0;
		while (j < fdf->max_y)
		{
			if (i < fdf->max_x - 1)
				plot(fdf->map_data[i][j], fdf->map_data[i + 1][j], img, fdf);
			if (j < fdf->max_y - 1)
				plot(fdf->map_data[i][j], fdf->map_data[i][j + 1], img, fdf);
			j++;
		}
		i++;
	}

	mlx_resize_image(img, mlx->width, mlx->height);
	mlx_image_to_window(mlx, img, 0, 0);
	mlx_key_hook(mlx, &key_func, mlx);
	mlx_loop(mlx);
	free_map(fdf);
	free(fdf);
	return (0);
}