/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   calc_map.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: udelorme <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/03/11 11:25:04 by udelorme          #+#    #+#             */
/*   Updated: 2016/03/15 11:29:08 by udelorme         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "calc_map.h"
#include "print_map.h"
#include "mlx.h"
#include "events.h"

int			get_iso_decal(t_global *global)
{
	t_map *map;

	map = global->map;
	while (map->next)
		map = map->next;
	return (ABSOL(map->p[0].x));
}

void		decal(t_map *map, int value_up, int value_down)
{
	int		i;
	t_map	*index;

	i = 0;
	index = map;
	while (index)
	{
		while (index->p[i].next == 1)
		{
			index->p[i].y += value_up;
			index->p[i].x += value_down;
			i++;
		}
		index->p[i].y += value_up;
		index->p[i].x += value_down;
		index = index->next;
		i = 0;
	}
}

void		change_pitch(t_global *global, int coef)
{
	int		i;

	i = 0;
	while (global->map)
	{
		while (global->map->p[i].next == 1)
		{
			global->map->p[i].y += (global->map->p[i].pitch * coef) / global->map->padding;
			i++;
		}
		global->map->p[i].y += ((global->map->p[i].pitch * coef) / global->map->padding);
		i = 0;
		global->map = global->map->next;
	}
}

void		up_tilt_map(t_global *global)
{
	t_map	*index;
	int		i;

	index = global->map;
	i = 0;
	if (global->map->inc > 0)
	{
		decal(global->map, -(index->p[global->map_lines / 2].y) * 0.5, 0);
		while (index)
		{
			while (index->p[i].next)
			{
				index->p[i].y /= 0.5;
				i++;
			}
			index->p[i].y /= 0.5;
			i = 0;
			index = index->next;
		}
		global->map->inc -= 1;
	}
}

void		down_tilt_map(t_global *global)
{
	t_map	*index;
	int		i;

	index = global->map;
	i = 0;
	if (global->map->inc < 10)
	{
		decal(global->map, index->p[global->map_lines / 2].y, 0);
		while (index)
		{
			while (index->p[i].next)
			{
				index->p[i].y *= 0.5;
				i++;
			}
			index->p[i].y *= 0.5;
			i = 0;
			index = index->next;
		}
		global->map->inc += 1;
	}
}

void		reload_map(t_global *global, int zoom, int i, t_coord margin)
{
	global->map->zoom_maj += zoom;
	global->map->pitch_maj += i;
	erase_map(global->env);
	trace_map(global->env, global->map, global->map->pitch_maj, margin, global);
	mlx_put_image_to_window(global->env.mlx, global->env.wnd, global->env.img.img_ptr, 0, 0);
	print_infos(global);
}
