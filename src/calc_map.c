/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   calc_map.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: udelorme <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/03/11 11:25:04 by udelorme          #+#    #+#             */
/*   Updated: 2016/03/11 16:16:02 by udelorme         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "calc_map.h"
#include "print_map.h"
#include "mlx.h"
#include "events.h"

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

t_coord		apply_pitch(t_coord p, int coef, t_global *global)
{
	t_coord pitched;

	pitched.x = p.x;
	pitched.y = p.y - ((p.pitch * ((coef * (global->map->padding)))) * 0.01);
	return (pitched);
}

int			calc_padding(t_map *map, int zoom)
{
	int		biggest_line;
	int		lines;
	int		padding;
	t_map	*index;
	int		i;

	lines = 0;
	index = map;
	padding = 1;
	biggest_line = 0;
	while (map)
	{
		if (biggest_line < map->size_line)
			biggest_line = map->size_line;
		map = map->next;
		lines++;
	}
	padding = HEIGHT / lines;
	if (!padding)
		padding = 1;
	padding *= zoom;
	i = 0;
	while (index)
	{
		while (index->p[i].next)
		{
			index->p[i].x *= padding;
			index->p[i].y *= padding;
			i++;
		}
		index->p[i].x *= padding;
		index->p[i].y *= padding;
		i = 0;
		index = index->next;
	}
	return (ABSOL(padding));
}

double		dec_padding(t_global *global, int zoom, int x, int y)
{
	int		lines;
	double	padding;
	t_map	*index;
	int		i;

	lines = 0;
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
	int		lines;
	double	padding;
	t_map	*index;
	int		i;

	lines = 0;
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
