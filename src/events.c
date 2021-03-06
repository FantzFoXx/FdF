/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   events.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: udelorme <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/03/11 11:34:57 by udelorme          #+#    #+#             */
/*   Updated: 2016/03/16 09:18:56 by udelorme         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mlx.h"
#include "calc_map.h"
#include "libft.h"
#include "t_map.h"

int			my_mouse_hook(int button, int x, int y, t_global *global)
{
	if (button == 4)
	{
		global->map->padding = inc_padding(global, 2, x, y);
		reload_map(global, 0, 0);
	}
	else if (button == 5)
	{
		global->map->padding = dec_padding(global, 2, x, y);
		reload_map(global, 0, 0);
	}
	return (0);
}

static void	my_key_hook_suite(int key_code, t_global *global)
{
	if (key_code == 0)
	{
		global->map->margin.x = -10;
		decal(global->map, global->map->margin.y, global->map->margin.x);
	}
	else if (key_code == 2)
	{
		global->map->margin.x = 10;
		decal(global->map, global->map->margin.y, global->map->margin.x);
	}
	else if (key_code == 69 && global->map->padding < 999999999)
		global->map->padding = inc_padding(global, 2, -1, -1);
	else if (key_code == 78 && global->map->padding > 0.01)
		global->map->padding = dec_padding(global, 2, -1, -1);
	else if (key_code == 3)
		up_tilt_map(global);
	else if (key_code == 15)
		down_tilt_map(global);
	else if (key_code == 53)
		free_global(global);
}

int			my_key_hook(int key_code, t_global *global)
{
	global->map->margin.x = 0;
	global->map->margin.y = 0;
	if (key_code == 125)
		reload_map(global, 0, -1);
	else if (key_code == 126)
		reload_map(global, 0, 1);
	else if (key_code == 13)
	{
		global->map->margin.y = -10;
		decal(global->map, global->map->margin.y, global->map->margin.x);
	}
	else if (key_code == 1)
	{
		global->map->margin.y = 10;
		decal(global->map, global->map->margin.y, global->map->margin.x);
	}
	else
		my_key_hook_suite(key_code, global);
	if (key_code != (125 & 126))
		reload_map(global, 0, 0);
	return (0);
}

void		print_infos(t_global *global)
{
	char *info;
	char *tmp;

	info = ft_strjoin("Map    : ", global->env.map_name);
	mlx_string_put(global->env.mlx, global->env.wnd, 20, 20, 0x00FFFFFF, info);
	free(info);
	tmp = ft_itoa(WIDTH);
	info = ft_strjoin("Width  : ", tmp);
	free(tmp);
	mlx_string_put(global->env.mlx, global->env.wnd, 20, 40, 0x00FFFFFF, info);
	free(info);
	tmp = ft_itoa(HEIGHT);
	info = ft_strjoin("Height : ", tmp);
	free(tmp);
	mlx_string_put(global->env.mlx, global->env.wnd, 20, 60, 0x00FFFFFF, info);
	free(info);
	tmp = ft_itoa((int)global->map->padding);
	info = ft_strjoin("Zoom   : ", tmp);
	free(tmp);
	mlx_string_put(global->env.mlx, global->env.wnd, 20, 80, 0x00FFFFFF, info);
	free(info);
	tmp = ft_itoa((int)global->map->pitch_maj);
	info = ft_strjoin("Pitch  : ", tmp);
	free(tmp);
	mlx_string_put(global->env.mlx, global->env.wnd, 20, 100, 0x00FFFFFF, info);
}

void		free_global(t_global *global)
{
	t_map	*map;
	t_map	*bak;

	map = global->map;
	while (map)
	{
		free(map->p);
		bak = map;
		map = map->next;
		free(bak);
	}
	exit(0);
}
