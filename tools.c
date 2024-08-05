/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tools.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: labderra <labderra@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/04 17:17:53 by labderra          #+#    #+#             */
/*   Updated: 2024/08/04 17:18:20 by labderra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

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
	int			i;

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
