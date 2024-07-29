/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: labderra <labderra@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/24 20:32:49 by labderra          #+#    #+#             */
/*   Updated: 2024/07/28 16:35:03 by labderra         ###   ########.fr       */
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

void	plot(t_point start, t_point end, mlx_image_t *img)
{
	float		slope;
	uint32_t	i;
	int			origin;

	i = start.y;
	if (end.x == start.x)
	{
		while (i++ < end.y)
			mlx_put_pixel(img, start.x, i, grad_color(start.c,
					end.c, (i - start.y + 0.0) / (end.y - start.y)));
		return ;
	}
	slope = (float)(end.y - start.y) / (float)(end.x - start.x);
	origin = start.y - slope * start.x;
	if (slope <= 1.000 || slope >= -1.000)
	{
		i = start.x;
		while (i++ < end.x)
			mlx_put_pixel(img, i, i * slope + origin, grad_color(start.c,
					end.c, (i - start.x + 0.0) / (end.x - start.x)));
		return ;
	}
	while (i++ < end.y)
		mlx_put_pixel(img, (i - origin) / slope, i, grad_color(start.c,
				end.c, (i - start.y + 0.0) / (end.y - start.y)));
}

void	key_func(mlx_key_data_t keydata, void *param)
{
	if (keydata.key == MLX_KEY_ESCAPE && keydata.action == MLX_PRESS)
		mlx_terminate((mlx_t *)param);
}

char	***read_fdf(char *f_name, char ***tmp_map, int *n_lines, int *line_len)
{
	int		fd;
	char	*tmp_line;

	tmp_map = (char ***)ft_calloc(1024, sizeof(char **));
	fd = open(f_name, O_RDONLY);
	if (!tmp_map || fd == -1)
		return (NULL);
	*n_lines = 0;
	tmp_line = get_next_line(fd);
	while (tmp_line)
	{
		tmp_map[*n_lines++] = ft_split(tmp_line, ' ');
		free(tmp_line);
		tmp_line = get_next_line(fd);
	}
	close (fd);
	*line_len = 0;
	while (tmp_map[0][*line_len])
		*line_len += 1;
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

	i = 0;
	while (matrix && matrix[i])
		free_split(matrix[i++]);
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

int	load_map(char *f_name, t_map fdf)
{
	char	***tmp_map;
	int		j;
	int		i;

	tmp_map = NULL;
	tmp_map = read_fdf(f_name, tmp_map, &fdf.max_x, &fdf.max_y);
	fdf.map_data = malloc(sizeof(t_point) * fdf.max_x * fdf.max_y);
	if (fdf.max_x == 0 || fdf.max_y == 0 || !tmp_map || !fdf.map_data)
		return (-1);
	i = -1;
	while (++i < fdf.max_x)
	{
		j = 0;
		while (j < fdf.max_y && tmp_map[i][j])
		{
			fdf.map_data[i][j].x = (i + j) * 0.87;
			fdf.map_data[i][j].y = (i - j) * 0.5 + ft_atoi(tmp_map[i][j]);
			fdf.map_data[i][j].c = load_color(tmp_map[i][j]);
			j++;
		}
		if (!(j < fdf.max_y && tmp_map[i][j]))
			return (-1);
	}
	return (free_matrix(tmp_map), 0);		
}

int	main(int argc, char **argv)
{
	mlx_t		*mlx;
	mlx_image_t	*img;
//	int			i;
//	int			j;
	t_map		*fdf;
	
/* 	t_point		origin = {0, 0, 0x00ffffff};
	t_point		end = {400, 200, 0xff0000ff}; */

	if (argc != 2 || !ft_strnstr(argv[1], ".fdf\0", ft_strlen(argv[1]) + 1))
		return (write(2, "Usage : ./fdf <filename.fdf>\n", 29), 1);
	fdf = malloc (sizeof(t_map));
	if (!fdf || load_map(argv[1], *fdf) == -1)
		return (write(2, "Error reading map or map invalid\n", 33), 1);
	mlx = mlx_init(400, 400, "FdF", 1);
	if (!mlx)
		return (1);
	img = mlx_new_image(mlx, 400, 400);
//	i = 0;
//	j = 0;
//	plot(origin, end, img);
	
/* 
	i = 10;
	while (i < 400)
	{
		origin.y = i;
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