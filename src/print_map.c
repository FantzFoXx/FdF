/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_map.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: udelorme <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/03/11 11:15:53 by udelorme          #+#    #+#             */
/*   Updated: 2016/03/14 17:47:36 by udelorme         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "print_map.h"
#include <stdlib.h>
#include "math.h"
#include "calc_map.h"
#include "mlx.h"
#include "libft.h"

void		img_put_pixel(t_img_prop img, int x, int y, int color)
{
	int				final;
	unsigned char	*col;

	col = (unsigned char *)&color;
	x *= (img.bits_per_pixel / 8);
	y *= img.size_line;
	final = x + y;
	img.img_addr[final + 0] = col[0];
	img.img_addr[final + 1] = col[1];
	img.img_addr[final + 2] = col[2];
	img.img_addr[final + 3] = col[3];
}

void	foo1(t_global *global, t_coord index, t_coord d_point, double pitch)
{
	int cumul;
	int i;

	cumul = d_point.x / 2;
	i = 1;
	while (i <= d_point.x)
	{
		index.x += global->env.img.xinc;
		cumul += d_point.y;
		if (cumul >= d_point.x)
		{
			cumul -= d_point.x;
			index.y += global->env.img.yinc;
		}
		if ((index.x >= 0 && index.y >= 0)
			&& (index.x < WIDTH && index.y < HEIGHT))
			img_put_pixel(global->env.img, index.x
				, index.y, pitch_color(pitch, global));
		i++;
	}
}

void	foo2(t_global *global, t_coord index, t_coord d_point, double pitch)
{
	int cumul;
	int i;

	cumul = d_point.y / 2;
	i = 1;
	while (i <= d_point.y)
	{
		index.y += global->env.img.yinc;
		cumul += d_point.x;
		if (cumul >= d_point.y)
		{
			cumul -= d_point.y;
			index.x += global->env.img.xinc;
		}
		if ((index.x >= 0 && index.y >= 0)
			&& (index.x < WIDTH && index.y < HEIGHT))
			img_put_pixel(global->env.img, index.x
				, index.y, pitch_color(pitch, global));
		i++;
	}
}

void	draw_segment(t_img_prop img, t_coord point_a, t_coord point_b, double pitch, t_global *global) // replace img by global
{
	t_coord d_point;
	t_coord index;

	index.x = floor(point_a.x);
	index.y = floor(point_a.y);
	d_point.x = floor(point_b.x) - floor(point_a.x);
	d_point.y = floor(point_b.y) - floor(point_a.y);
	global->env.img.xinc = (d_point.x > 0) ? 1 : -1;
	global->env.img.yinc = (d_point.y > 0) ? 1 : -1;
	d_point.x = ABSOL(floor(d_point.x));
	d_point.y = ABSOL(floor(d_point.y));
	if ((index.x >= 0 && index.y >= 0) && (index.x < WIDTH && index.y < HEIGHT))
		img_put_pixel(img, index.x, index.y, pitch_color(pitch, global));
	if (d_point.x > d_point.y)
		foo1(global, index, d_point, pitch);
	else
		foo2(global, index, d_point, pitch);
}

void	trace_map(t_meta env, t_map *map, int coef, t_coord margin, t_global *global)
{
	int i;

	(void)margin;
	i = 0;
	if (map && !map->next)
		while (map->p[i].next == 1)
		{
			if ((map->p[i].x < WIDTH || map->p[i].y < HEIGHT)
					&& (map->p[i].x >= 0 || map->p[i].y >= 0))
				draw_segment(env.img, apply_pitch(map->p[i], coef, global)
					, apply_pitch(map->p[i + 1], coef, global), map->p[i].pitch, global);
			i++;
		}
	else
		while (map->next)
		{
			while (map->p[i].next == 1)
			{
				if ((map->p[i].x < WIDTH || map->p[i].y < HEIGHT)
						&& (map->p[i].x >= 0 || map->p[i].y >= 0))
				{
					draw_segment(env.img, apply_pitch(map->p[i], coef, global)
					, apply_pitch(map->p[i + 1], coef, global), map->p[i].pitch, global);
					draw_segment(env.img, apply_pitch(map->p[i], coef, global)
					, apply_pitch(map->next->p[i], coef, global), map->p[i].pitch, global);
				}
				if ((map->p[i].x < WIDTH || map->p[i].y < HEIGHT)
						&& (map->p[i].x >= 0 || map->p[i].y >= 0))
					draw_segment(env.img, apply_pitch(map->next->p[i], coef, global)
					, apply_pitch(map->next->p[i + 1], coef, global), map->p[i].pitch, global);
				i++;
			}
			if ((map->p[i].x < WIDTH || map->p[i].y < HEIGHT)
					&& (map->p[i].x >= 0 || map->p[i].y >= 0))
				draw_segment(env.img, apply_pitch(map->p[i], coef, global)
				, apply_pitch(map->next->p[i], coef, global), map->p[i].pitch, global);
			i = 0;
			map = map->next;
		}
}

int		calc_rgb(int r, int g, int b)
{
	int ret;

	ret = b;
	ret += g * 0x100;
	ret += r * 0x10000;
	return (ret);
}

int		pitch_color(int pitch, t_global *global)
{
	if (pitch >= ((global->high_pitch * 50) / 100))
		return (global->colors[4]);
	else if (pitch >= ((global->high_pitch * 15) / 100))
		return (global->colors[3]);
	else if (pitch >= ((global->high_pitch * 10) / 100) || pitch > 0)
		return (global->colors[2]);
	else if (pitch == 0)
		return (global->colors[1]);
	else if (pitch < 0)
		return (global->colors[0]);
	return (0x00FFFFFF);
}

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
