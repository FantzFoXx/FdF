/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   calc_map.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: udelorme <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/03/11 11:25:04 by udelorme          #+#    #+#             */
/*   Updated: 2016/03/15 16:00:59 by udelorme         ###   ########.fr       */
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

void		calc_iso(t_map *map, t_global *global)
{
	t_map	*bak;
	int		i;
	int		x_bak;
	int		y_bak;

	i = 0;
	x_bak = 0;
	y_bak = 0;
	bak = map;
	while (map)
	{
		while (map->p[i].next == 1)
		{
			map->p[i].x = ((map->p[i].x - map->p[i].y) / 2);
			map->p[i].y = ((map->p[i].y + map->p[i].x) / 1.5);
			i++;
		}
		map->p[i].x = ((map->p[i].x - map->p[i].y) / 2);
		map->p[i].y = ((map->p[i].y + map->p[i].x) / 1.5);
		i = 0;
		map = map->next;
	}
	decal(bak, 0, get_iso_decal(global));
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
