/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_map_2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: udelorme <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/03/15 16:01:22 by udelorme          #+#    #+#             */
/*   Updated: 2016/03/16 10:41:00 by udelorme         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "print_map.h"
#include "calc_map.h"
#include "mlx.h"

void	erase_map(t_meta env)
{
	int i;
	int j;

	i = 0;
	j = 0;
	while (i < WIDTH)
	{
		j = 0;
		while (j < HEIGHT)
		{
			img_put_pixel(env.img, i, j, 0x00000000);
			j++;
		}
		i++;
	}
	mlx_put_image_to_window(env.mlx, env.wnd, env.img.img_ptr, 0, 0);
}

double	cp(t_map *map, int nxt, int i)
{
	if (nxt)
	{
		if (map->p[i].pitch > map->next->p[i].pitch)
			return (map->p[i].pitch);
		else
			return (map->next->p[i].pitch);
	}
	else
	{
		if (map->p[i].pitch >= map->p[i + 1].pitch)
			return (map->p[i].pitch);
		else if (map->p[i].next)
			return (map->p[i + 1].pitch);
		else
			return (map->p[i].pitch);
	}
}

void	trace_map_2(t_map *map, int coef, t_global *global, int i)
{
	while (map->p[i].next == 1)
	{
		if ((map->p[i].x < WIDTH || map->p[i].y < HEIGHT)
				&& (map->p[i].x >= 0 || map->p[i].y >= 0))
		{
			draw_segment(apply_pitch(map->p[i], coef, global), apply_pitch(
				map->p[i + 1], coef, global), cp(map, 0, i), global);
			draw_segment(apply_pitch(map->p[i], coef, global), apply_pitch(
				map->next->p[i], coef, global), cp(map, 1, i), global);
		}
		if ((map->p[i].x < WIDTH || map->p[i].y < HEIGHT)
				&& (map->p[i].x >= 0 || map->p[i].y >= 0))
			draw_segment(apply_pitch(map->next->p[i], coef, global)
				, apply_pitch(map->next->p[i + 1], coef, global)
					, cp(map, 1, i), global);
		i++;
	}
	if ((map->p[i].x < WIDTH || map->p[i].y < HEIGHT)
			&& (map->p[i].x >= 0 || map->p[i].y >= 0))
		draw_segment(apply_pitch(map->p[i], coef, global), apply_pitch(
			map->next->p[i], coef, global), cp(map, 0, i), global);
	i = 0;
}

void	trace_map(t_map *map, int coef, t_global *global)
{
	int i;

	i = 0;
	if (map && !map->next)
		while (map->p[i].next == 1)
		{
			if ((map->p[i].x < WIDTH || map->p[i].y < HEIGHT)
					&& (map->p[i].x >= 0 || map->p[i].y >= 0))
				draw_segment(apply_pitch(map->p[i], coef, global), apply_pitch(
					map->p[i + 1], coef, global), map->p[i].pitch, global);
			i++;
		}
	else
		while (map->next)
		{
			trace_map_2(map, coef, global, i);
			map = map->next;
		}
}
