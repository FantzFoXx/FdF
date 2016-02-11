#include <unistd.h>
#include "fdf.h"
#include <mlx.h>
#include <math.h>
#include "libft.h"
#include <stdio.h>
#include <stdlib.h>

t_pitch_map		*t_pitch_new(char *line)
{
	t_pitch_map *new;

	new = (t_pitch_map *)malloc(sizeof(t_pitch_map));
	if (new)
	{
		ft_trace("split_b", "pass");
		new->line = ft_strsplit(line, ' ');
		ft_trace("split_a", "pass");
		new->next = NULL;
	}
	return (new);
}

void		t_pitch_push(t_pitch_map **first, t_pitch_map *new)
{
	t_pitch_map *index;

	index = *first;
	if (!index)
		*first = new;
	else
	{
		while (index->next)
			index = index->next;
		index->next = new;
	}
}

t_coord		*t_coord_new(int x, int y, int index)
{
	t_coord *new;

	new = (t_coord *)malloc(sizeof(t_coord));
	if (new)
	{
		new->x = x;
		new->y = y;
		new->index = index;
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

void	draw_segment(void *mlx, void *wnd, t_coord *point_a, t_coord *point_b, int color)
{
	t_coord d_point;
	t_coord index;
	int	i;
	int	xinc;
	int	yinc;
	int	cumul;

	index.x = point_a->x;
	index.y = point_a->y;
	d_point.x = point_b->x - point_a->x;
	d_point.y = point_b->y - point_a->y;
	xinc = ( d_point.x > 0 ) ? 1 : -1;
	yinc = ( d_point.y > 0 ) ? 1 : -1;
	d_point.x = ABSOL(d_point.x);
	d_point.y = ABSOL(d_point.y);
	mlx_pixel_put(mlx, wnd, index.x, index.y, color);
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
			mlx_pixel_put(mlx, wnd, index.x, index.y, color);
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
			mlx_pixel_put(mlx, wnd, index.x, index.y, color);
		}
	}
}

void		link_dots(t_coord *map)
{
	if (map->right && map->right->right)
		link_dots(map->right);
	if (map->right && map->down)
	{
		map->down->right = map->right->down;
		link_dots(map->down);
	}
}

t_coord		*create_map(t_coord *map, int x_dots, int y_dots, int space)
{
	int		x;
	int		y;
	t_coord *index;
	int		first_map;
	int		ind;
	int		i;

	x = 0;
	y = 0;
	ind = 0;
	i = 0;
	first_map = 1;
	while (x <= x_dots)
	{
		t_coord_push_right(&index, t_coord_new(space * x + 50 + i, (space / 1.5) * y + 50 + i, ind++));
		if (first_map)
			map = index;
		first_map = 0;
		x++;
		i += 2;
	}
	x = 0;
	while (index)
	{
		i = 0;
		y = 1;
		while (y <= y_dots)
		{
			i += 15;
			t_coord_push_down(&index, t_coord_new(space * x + 50 + i, (space / 1.5) * y + 50 + i, ind++));
			y++;
		}
		index = index->right;
		x++;
	}
	link_dots(map);
	return (map);
}

static void	ab_segment(t_meta env, t_coord *map)
{
	int i;
	t_coord *index;
	index = map;
	i = 0;
	while (map)
	{
		index = map;
		while (index)
		{
			if (index->right)
				draw_segment(env.mlx, env.wnd, index, index->right, 0x33CCFF);
			if (index->down)
				draw_segment(env.mlx, env.wnd, index, index->down, 0x33CCFF);
			index = index->down;
			i++;
		}
		map = map->right;
	}
}

void	print_map(t_meta env, t_coord *map)
{
	int i;
	t_coord *index;
	index = map;
	i = 255;
	while (map)
	{
		index = map;
		while (index)
		{
			mlx_pixel_put(env.mlx, env.wnd, index->x + 50, index->y + 50, i--);
			index = index->down;
		}
		map = map->right;
	}
}

void	apply_pitch_map(t_coord *map, t_pitch_map *pitch)
{
	t_coord		*index;
	int			i;

	index = map;
	i = 0;
	while (map && pitch)
	{
		ft_trace(NULL, "pass");
		index = map;
		i = 0;
		while (index && pitch->line[i])
		{
			ft_nbrtrace("atoi", ft_atoi(pitch->line[i]));
			index->y -= (ft_atoi(pitch->line[i]) * 3);
			index->y -= 0;
			index = index->right;
			i++;
		}
		pitch = pitch->next;
		map = map->down;
	}
}

int		main(int argc, char **argv)
{
	t_meta	env;
	t_coord	begin;
	t_coord *map;
	t_pitch_map *pitch;
	char *line;
	int fd;
	int	line_size;
	int	nb_line;

	map = NULL;
	begin.x = 0;
	begin.y = 0;
	line_size = 10;
	nb_line = 10;
	env.mlx = mlx_init();
	env.wnd = mlx_new_window(env.mlx, WIDTH, HEIGHT, "Test 1");
	fd = 0;
	line = NULL;
	pitch = NULL;
	if (argc == 2)
	{
		fd = open(argv[1], O_RDONLY);
		if (fd)
		{
			line_size = 0;
			nb_line = 0;
			ft_trace("fd", "pass");
			while (get_next_line(fd, &line) > 0)
			{
				nb_line++;
				ft_trace("gnl", "pass");
				line_size = ft_wordcount(line, ' ');
				t_pitch_push(&pitch, t_pitch_new(line));
				free(line);
			}
		}
		close(fd);
	}
	map = create_map(map, line_size, nb_line, 70);
	apply_pitch_map(map, pitch);
	//print_map(env, map);
	ab_segment(env, map);
	mlx_loop(env.mlx);
	return (0);
}
