/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initialize.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: labderra <labderra@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/04 17:14:46 by labderra          #+#    #+#             */
/*   Updated: 2024/08/04 19:49:27 by labderra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

static void	set_o_xy(t_map *fdf)
{
	int	i;
	int	j;

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
	fdf->scale = (fdf->img_width * 0.9) / (fdf->x_rng[1] - fdf->x_rng[0]);
	if (((fdf->img_height - 10.0) / (fdf->y_rng[1] - fdf->y_rng[0]))
		< fdf->scale)
		fdf->scale = (fdf->img_height * 0.9) / (fdf->y_rng[1] - fdf->y_rng[0]);
	fdf->o_xy[0] = fdf->img_width / 2.0 - (fdf->x_rng[1] + fdf->x_rng[0])
		* fdf->scale / 2.0;
	fdf->o_xy[1] = fdf->img_height / 2.0 - (fdf->y_rng[1] + fdf->y_rng[0])
		* fdf->scale / 2.0 - fdf->img_height * 0.025;
}

static int	check_map(t_map *fdf, char ***tmp_map)
{
	int	i;

	while (tmp_map[0] && tmp_map[0][fdf->max_x])
		fdf->max_x++;
	i = fdf->max_y;
	while (i--)
		if (!tmp_map[i]
			|| !tmp_map[i][fdf->max_x - 1] || tmp_map[i][fdf->max_x])
			return (-1);
	return (0);
}

static char	***read_fdf(t_map *fdf)
{
	int		fd;
	char	*tmp_line;
	char	***tmp_map;

	tmp_map = (char ***)ft_calloc(1024, sizeof(char **));
	fd = open(fdf->filename, O_RDONLY);
	if (!tmp_map || fd == -1)
		return (free(tmp_map), NULL);
	tmp_line = get_next_line(fd);
	while (tmp_line)
	{
		tmp_map[fdf->max_y] = ft_split(tmp_line, ' ');
		free(tmp_line);
		if (!tmp_map[fdf->max_y++])
			return (free_matrix(tmp_map), NULL);
		tmp_line = get_next_line(fd);
	}
	close (fd);
	if (check_map(fdf, tmp_map) == -1)
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
		return (free_matrix(tmp_map), free_map(fdf), free(fdf), -1);
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
			fdf->map_data[i][j].y = (i + j) * 50 - 87 * ft_atoi(tmp_map[j][i]);
			fdf->map_data[i][j].c = load_color(tmp_map[j][i]);
		}
	}
	set_o_xy(fdf);
	return (free_matrix(tmp_map), 0);
}

int	init_fdf(t_map **fdf, char *filename)
{
	*fdf = ft_calloc(sizeof(t_map), 1);
	if (!(*fdf))
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
	(*fdf)->scale = 1.0;
	(*fdf)->o_xy[0] = 0;
	(*fdf)->o_xy[1] = 0;
	(*fdf)->map_data = NULL;
	return (0);
}
