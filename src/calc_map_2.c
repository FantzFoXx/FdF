/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   calc_map_2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: udelorme <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/03/15 11:26:52 by udelorme          #+#    #+#             */
/*   Updated: 2016/03/15 16:00:24 by udelorme         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "calc_map.h"

int			calc_padding(t_map *map, int zoom, t_global *global)
{
	int		padding;
	int		i;

	padding = HEIGHT / global->map_lines;
	if (!padding)
		padding = 1;
	padding *= zoom;
	i = 0;
	while (map)
	{
		while (map->p[i].next)
		{
			map->p[i].x *= padding;
			map->p[i].y *= padding;
			i++;
		}
		map->p[i].x *= padding;
		map->p[i].y *= padding;
		i = 0;
		map = map->next;
	}
	return (ABSOL(padding));
}

double		dec_padding(t_global *global, int zoom, int x, int y)
{
	double	padding;
	t_map	*index;
	int		i;

	index = global->map;
	padding = global->map->padding / zoom;
	i = 0;
	while (index)
	{
		while (index->p[i].next)
		{
			index->p[i].x /= zoom;
			index->p[i].y /= zoom;
			i++;
		}
		index->p[i].x /= zoom;
		index->p[i].y /= zoom;
		i = 0;
		index = index->next;
	}
	if (x >= 0 && y >= 0)
		decal(global->map, y / 2, x / 2);
	else
		decal(global->map, (HEIGHT / 4), (WIDTH / 4));
	return (padding);
}

double		inc_padding(t_global *global, int zoom, int x, int y)
{
	double	padding;
	t_map	*index;
	int		i;

	index = global->map;
	padding = global->map->padding * zoom;
	i = 0;
	while (index)
	{
		while (index->p[i].next)
		{
			index->p[i].x *= zoom;
			index->p[i].y *= zoom;
			i++;
		}
		index->p[i].x *= zoom;
		index->p[i].y *= zoom;
		i = 0;
		index = index->next;
	}
	if (x >= 0 && y >= 0)
		decal(global->map, -y, -x);
	else
		decal(global->map, -(HEIGHT / 2), -(WIDTH / 2));
	return (padding);
}

t_coord		apply_pitch(t_coord p, int coef, t_global *global)
{
	t_coord pitched;

	pitched.x = p.x;
	pitched.y = p.y - ((p.pitch * ((coef * (global->map->padding)))) * 0.01);
	return (pitched);
}

void		change_pitch(t_global *global, int coef)
{
	int		i;

	i = 0;
	while (global->map)
	{
		while (global->map->p[i].next == 1)
		{
			global->map->p[i].y += (global->map->p[i].pitch * coef)
				/ global->map->padding;
			i++;
		}
		global->map->p[i].y += ((global->map->p[i].pitch * coef)
				/ global->map->padding);
		i = 0;
		global->map = global->map->next;
	}
}
