/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: labderra <labderra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/24 20:32:49 by labderra          #+#    #+#             */
/*   Updated: 2024/07/30 14:04:07 by labderra         ###   ########.fr       */
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

void	plot(t_point start, t_point end, mlx_image_t *img, int sf)
{
	float		slope;
	uint32_t	i;
	int			origin;

	i = start.y * sf;
	if (end.x == start.x)
	{
		while (i++ < end.y * sf)
			mlx_put_pixel(img, start.x * sf, i, grad_color(start.c,
					end.c, (i - start.y * sf + 0.0) / (end.y - start.y)));
		return ;
	}
	slope = (float)(end.y - start.y) / (float)(end.x - start.x);
	origin = (start.y - slope * start.x) * sf;
	if (slope <= 1.000 || slope >= -1.000)
	{
		i = start.x * sf;
		while (i++ < end.x *sf)
			mlx_put_pixel(img, i, i * slope + origin, grad_color(start.c,
					end.c, (i - start.x * sf + 0.0) / (end.x - start.x)));
		return ;
	}
	while (i++ < end.y * sf)
		mlx_put_pixel(img, (i - origin) / slope, i, grad_color(start.c,
				end.c, (i - start.y *sf + 0.0) / (end.y - start.y)));
}

void	key_func(mlx_key_data_t keydata, void *param)
{
	if (keydata.key == MLX_KEY_ESCAPE && keydata.action == MLX_PRESS)
		mlx_terminate((mlx_t *)param);
}

char	***read_fdf(t_map *fdf)
{
	int		fd;
	char	*tmp_line;
	char	***tmp_map;

	tmp_map = (char ***)malloc(1024 * sizeof(char **));
	fd = open(fdf->filename, O_RDONLY);
	if (!tmp_map || fd == -1)
		return (NULL);
	fdf->max_y = 0;
	tmp_line = get_next_line(fd);
	while (tmp_line)
	{
		tmp_map[fdf->max_y++] = ft_split(tmp_line, ' ');
		free(tmp_line);
		tmp_line = get_next_line(fd);
	}
	tmp_map[fdf->max_y] = ft_calloc(sizeof(char **), 1);
	close (fd);
	fdf->max_x = 0;
	while (tmp_map[0][fdf->max_x])
		fdf->max_x++;
	return (tmp_map);
}

void	free_split(char **str)
{
	int	i;

	i = 0;
	while (str && str[i])
		free(str[i++]);
	free(str);
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
	
	if (ft_strchr(str, 'x'))
	{
		tmp_color = ft_split(str, 'x');
		if (!tmp_color || !tmp_color[1])
		{
			perror("color parse fail");
			return (0xffffffff);
		}
		return (free_split(tmp_color), atoi_x(tmp_color[1]) * 256 + 0xff);
	}
	else
		return (0xffffffff);
}

int	check_square_map(char ***tmp_map, t_map *fdf)
{
	int	i;
	
	i = 0;
	while(i < fdf->max_y)
	{
		if (!(tmp_map[i][fdf->max_x - 1] && !tmp_map[i][fdf->max_x]))
			return (-1);
		i++;
	}
	return (0);
}

int	load_map(t_map *fdf)
{
	char	***tmp_map;
	int		j;
	int		i;

	tmp_map = read_fdf(fdf);
	if (fdf->max_x == 0 || fdf->max_y == 0 || !tmp_map
			|| check_square_map(tmp_map, fdf) == -1)
		return (free_matrix(tmp_map), -1);
	fdf->map_data = malloc(sizeof(t_point *) * fdf->max_y);
	if (!fdf->map_data)
		return (free_matrix(tmp_map), -1);
	i = -1;
	while (++i < fdf->max_y)
	{
		fdf->map_data[i] = malloc(sizeof(t_point) * fdf->max_x);
		if (!fdf->map_data[i])
			return (free_matrix(tmp_map), -1);
		j = 0;
		while (j < fdf->max_x && tmp_map[i][j])
		{
			fdf->map_data[i][j].x = (i + j) * 0.87;
			fdf->map_data[i][j].y = (i - j) * 0.5 + ft_atoi(tmp_map[i][j]);
			fdf->map_data[i][j].c = load_color(tmp_map[i][j]);
			j++;
		}
	}
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
	(*fdf)->img_width = 400;
	(*fdf)->img_height = 400;
	(*fdf)->scale_factor = 10;
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
		return (write(2, "Error reading map or Bad map\n", 33), free(fdf), 1);


	mlx = mlx_init(400, 400, "FdF", 1);
	if (!mlx)
		return (1);
	img = mlx_new_image(mlx, fdf->img_width, fdf->img_height);


//	plot(origin, end, img, fdf->scale_factor);
	i = 0;

	while (i < fdf->max_x - 1)
	{
		j = 0;
		while (j < fdf->max_y - 1)
		{
			printf("(%u, %u)\n", fdf->map_data[i][j].x, fdf->map_data[i][j].y);
/* 			plot(fdf->map_data[i][j], fdf->map_data[i + 1][j],
				img, fdf->scale_factor);
			plot(fdf->map_data[i][j], fdf->map_data[i][j + 1],
				img, fdf->scale_factor); */
			j++;
		}
		i++;
	}


/*		origin.y = i;
		end.y = i;
		plot(origin, end, img);
		i += 38;		
	}
	origin.x = 0;
	origin.y = 0;
	end.x = 0;
	end.y = 400;
	i = 10;
	while (i < 400)
	{
		origin.x = i;
		end.x = i;
		plot(origin, end, img);
		i += 60;		
	}
	 */
	
	mlx_resize_image(img, mlx->width, mlx->height);
	mlx_image_to_window(mlx, img, 0, 0);
	mlx_key_hook(mlx, &key_func, mlx);
	mlx_loop(mlx);
	return (0);
}