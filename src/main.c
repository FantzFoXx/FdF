#include <unistd.h>
#include "fdf.h"
#include <mlx.h>
#include <math.h>
#include "libft.h"
#include <stdio.h>
#include <stdlib.h>

t_coord		*t_coord_new(int x, int y)
{
	t_coord *new;

	new = (t_coord *)malloc(sizeof(t_coord));
	if (new)
	{
		new->x = x;
		new->y = y;
		new->right = NULL;
		new->down = NULL;
	}
	return (new);
}

void		t_coord_push_right(t_coord **first, t_coord *new)
{
	t_coord *index;

	index = *first;
	if (!index)
		*first = new;
	else
	{
		while (index->right)
			index = index->right;
		index->right = new;
	}
}

void		t_coord_push_down(t_coord **first, t_coord *new)
{
	t_coord *index;

	index = *first;
	if (!index)
		*first = new;
	else
	{
		while (index->down)
			index = index->down;
		index->down = new;
	}
}

void	ligne(void *mlx, void *wnd, t_coord point_a, t_coord point_b)
{
	t_coord d_point;
	t_coord index;
	int	i;
	int	xinc;
	int	yinc;
	int	cumul;

	index.x = point_a.x;
	index.y = point_a.y;
	d_point.x = point_b.x - point_a.x;
	d_point.y = point_b.y - point_a.y;
	xinc = ( d_point.x > 0 ) ? 1 : -1;
	yinc = ( d_point.y > 0 ) ? 1 : -1;
	d_point.x = ABSOL(d_point.x);
	d_point.y = ABSOL(d_point.y);
	mlx_pixel_put(mlx, wnd, index.x, index.y, 255);
	if (d_point.x > d_point.y)
	{
		cumul = d_point.x / 2;
		for (i = 1 ; i <= d_point.x ; i++) 
		{
			index.x += xinc;
			cumul += d_point.y;
			if (cumul >= d_point.x) 
			{
				cumul -= d_point.x;
				index.y += yinc; 
			}
			mlx_pixel_put(mlx, wnd, index.x, index.y, 255);
		} 
	}
	else 
	{
		cumul = d_point.y / 2;
		for (i = 1 ; i <= d_point.y ; i++) 
		{
			index.y += yinc;
			cumul += d_point.x;
			if (cumul >= d_point.y) 
			{
				cumul -= d_point.y;
				index.x += xinc; 
			}
			mlx_pixel_put(mlx, wnd, index.x, index.y, 255);
		}
	}
}

void		link_dots(t_coord *map)
{
	if (map->right)
	{
		if (map->right->right)
			link_dots(map->right);
		if (map->down)
		{
			map->down->right = map->right->down;
			link_dots(map->down);
		}
	}
}

t_coord		*create_map(t_coord *map, int x_dots, int y_dots, int space)
{
	t_coord *index;
	int		y_bak;

	y_bak = y_dots;
	index = t_coord_new(0, 0);
	map = index;
	while (x_dots >= 0)
	{
		y_dots = y_bak;
		while (y_dots >= 0)
		{
			t_coord_push_down(&index, t_coord_new(space * x_dots, space * y_dots));
			y_dots--;
		}
		t_coord_push_right(&index, t_coord_new(space * x_dots, space * y_dots));
		index = index->right;
		x_dots--;
	}
	link_dots(map);
	return (map);
}

void	print_map(t_meta env, t_coord *map)
{
	t_coord *index;
	index = map;
	while (map)
	{
		index = map;
		while (index)
		{
			mlx_pixel_put(env.mlx, env.wnd, index->x, index->y, 255);
			index = index->down;
		}
		map = map->right;
	}
}

int		main(void)
{
	t_meta	env;
	t_coord	begin;
	t_coord *map;

	map = NULL;
	begin.x = 100;
	begin.y = 100;
	env.mlx = mlx_init();
	env.wnd = mlx_new_window(env.mlx, WIDTH, HEIGHT, "Test 1");
	map = create_map(map, 10, 10, 20);
	print_map(env, map);
	mlx_loop(env.mlx);
	return (0);
}
