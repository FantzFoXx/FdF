/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: udelorme <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/03/11 16:47:51 by udelorme          #+#    #+#             */
/*   Updated: 2016/03/16 10:47:38 by udelorme         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include "t_map.h"
#include "fdf.h"
#include "mlx.h"
#include "calc_map.h"
#include "events.h"
#include "print_map.h"

void	create_map(t_global *global)
{
	global->map->padding =
		calc_padding(global->map, global->map->zoom_maj, global);
	calc_iso(global->map, global);
	global->map->margin.y = 20;
	global->map->margin.x = 20;
	decal(global->map, global->map->margin.y, global->map->margin.x);
	trace_map(global->map, global->map->pitch_maj, global);
	mlx_put_image_to_window(global->env.mlx, global->env.wnd
			, global->env.img.img_ptr, 0, 0);
	print_infos(global);
}

void	reload_map(t_global *global, int zoom, int i)
{
	global->map->zoom_maj += zoom;
	global->map->pitch_maj += i;
	erase_map(global->env);
	trace_map(global->map, global->map->pitch_maj, global);
	mlx_put_image_to_window(global->env.mlx, global->env.wnd
		, global->env.img.img_ptr, 0, 0);
	print_infos(global);
}

void	init_params(t_global *global)
{
	global->map->pitch_maj = 2;
	global->map->zoom_maj = 1;
	global->colors[0] = 0x00D0B15C;
	global->colors[1] = 0x00004FC6;
	global->colors[2] = 0x0083AB00;
	global->colors[3] = 0x006C4500;
	global->colors[4] = 0x00FFFFFF;
	global->map->inc = 0;
}

int		main(int argc, char **argv)
{
	t_global	global;
	t_meta		*env;
	int			fd;

	env = &global.env;
	fd = open_file(argc, argv);
	global.map = init_map(fd, argv[1], &global);
	close(fd);
	env->mlx = mlx_init();
	env->wnd = mlx_new_window(env->mlx, WIDTH, HEIGHT, "Fdf");
	env->img.img_ptr = mlx_new_image(env->mlx, WIDTH, HEIGHT);
	env->img.img_addr = mlx_get_data_addr(env->img.img_ptr
	, &(env->img.bits_per_pixel), &(env->img.size_line), &(env->img.endian));
	init_params(&global);
	create_map(&global);
	mlx_hook(env->wnd, 2, (1L << 0), &my_key_hook, &global);
	mlx_hook(env->wnd, 4, 0L, &my_mouse_hook, &global);
	mlx_loop(env->mlx);
	return (0);
}
