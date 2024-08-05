/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: labderra <labderra@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/24 20:32:49 by labderra          #+#    #+#             */
/*   Updated: 2024/08/04 19:47:54 by labderra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

static void	key_func(mlx_key_data_t keydata, void *param)
{
	if (keydata.key == MLX_KEY_ESCAPE && keydata.action == MLX_PRESS)
	{
		mlx_close_window((mlx_t *)param);
	}
}

int	main(int argc, char **argv)
{
	mlx_t		*mlx;
	mlx_image_t	*img;
	t_map		*fdf;

	if (argc != 2 || !ft_strnstr(argv[1], ".fdf\0", ft_strlen(argv[1]) + 1))
		return (write(2, "Usage : ./fdf <filename.fdf>\n", 29), 1);
	if (init_fdf(&fdf, argv[1]) == -1 || load_map(fdf) == -1)
		return (write(2, "Error reading map or Bad map\n", 29), 1);
	mlx = mlx_init(1000, 1000, "FdF", 1);
	if (!mlx)
		return (perror("mlx"), 1);
	img = mlx_new_image(mlx, fdf->img_width, fdf->img_height);
	if (!img)
		return (perror("mlx"), 1);
	plot_map(fdf, img);
	mlx_resize_image(img, mlx->width, mlx->height);
	mlx_image_to_window(mlx, img, 0, 0);
	mlx_key_hook(mlx, &key_func, mlx);
	mlx_loop(mlx);
	mlx_delete_image(mlx, img);
	mlx_terminate(mlx);
	free_map(fdf);
	free(fdf);
	return (0);
}
