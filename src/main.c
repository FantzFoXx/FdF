#include <unistd.h>
#include "fdf.h"
#include "mlx.h"
#include <math.h>
#include "libft.h"
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include "libft.h"
#include "get_next_line.h"
#include "catch_errors.h"
#include "t_map.h"

static void		img_put_pixel(t_img_prop img, int x, int y, int color)
{
	int	final;
	unsigned char *col;
	(void)color;

	col = (unsigned char *)&color;

	x *= (img.bits_per_pixel / 8);
	y *= img.size_line;

	final = x + y;
	img.img_addr[final + 0] = col[0];
	img.img_addr[final + 1] = col[1];
	img.img_addr[final + 2] = col[2];
	img.img_addr[final + 3] = col[3];
}

void	draw_segment(t_img_prop img, t_coord point_a, t_coord point_b, double pitch)
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
	if ((index.x >= 0 && index.y >= 0) && (index.x < WIDTH && index.y < HEIGHT))
		//img_put_pixel(img, index.x, index.y, color);
		img_put_pixel(img, index.x, index.y, RGB(127.5 * (cos(pitch) + 1), 127.5 * (sin(pitch) + 1), 127.5 * (1 - cos(pitch))));
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
			if ((index.x >= 0 && index.y >= 0) && (index.x < WIDTH && index.y < HEIGHT))
				img_put_pixel(img, index.x, index.y, RGB(127.5 * (cos(pitch) + 1), 127.5 * (sin(pitch) + 1), 127.5 * (1 - cos(pitch))));
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
			if ((index.x >= 0 && index.y >= 0) && (index.x < WIDTH && index.y < HEIGHT))
				//img_put_pixel(img, index.x, index.y, color);
				img_put_pixel(img, index.x, index.y, RGB(127.5 * (cos(pitch) + 1), 127.5 * (sin(pitch) + 1), 127.5 * (1 - cos(pitch))));
		}
	}
}

static t_map	*init_map(int fd)
{
	char *line;
	t_map *map;
	t_map *index;
	size_t	line_nb;

	map = NULL;
	line_nb = 0;
	index = map;
	while (get_next_line(fd, &line) > 0)
	{
		t_map_push(&map, t_map_new(line, line_nb));
		free(line);
		line_nb++;
	}

	return (map);
}
/*
   static void	calc_iso(t_map *map)
   {
   int i;

   i = 0;
   while (map)
   {
   while (map->p[i].next == 1)
   {
   map->p[i].x = ((((map->p[i].x * 2) + (map->p[i].y / 2))) / 2);
   map->p[i].y = ((((map->p[i].y * 2) + (map->p[i].x / 2))) / 2);
   i++;
   }
   map->p[i].x = ((((map->p[i].x * 2) + (map->p[i].y / 2))) / 2);
   map->p[i].y = ((((map->p[i].y * 2) + (map->p[i].x / 2))) / 2);
   i = 0;
   map = map->next;
   }
   }
   */

static void	calc_iso(t_map *map)
{
	int i;
	int x_bak;
	int y_bak;

	i = 0;
	x_bak = 0;
	y_bak = 0;
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
}

static void decal(t_map *map, int value_up, int value_down)
{
	int i;
	t_map *index;

	i = 0;
	index = map;
	ft_nbrtrace("value_up", value_up);
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

/*
   static void apply_pitch(t_map *map)
   {
   int i;

   i = 0;
   while (map)
   {
   while (map->p[i].next == 1)
   {
   map->p[i].y -= ((map->p[i].pitch * (map->size_line / 3)) / 2);
   i++;
   }
   map->p[i].y -= ((map->p[i].pitch * (map->size_line / 3)) / 2);
   i = 0;
   map = map->next;
   }

   }
   */
static t_coord apply_pitch(t_coord p, int coef)
{
	t_coord pitched;

	pitched.x = p.x;
	pitched.y = p.y - (p.pitch * coef);
	return (pitched);
}

static void trace_map(t_meta env, t_map *map, int coef, t_coord margin)
{
	int i;
	(void)margin;

	i = 0;
	while (map->next)
	{
		while (map->p[i].next == 1)
		{
			if ((map->p[i].x < WIDTH || map->p[i].y < HEIGHT)
					&& (map->p[i].x >= 0 || map->p[i].y >= 0))
			{
				draw_segment(env.img, apply_pitch(map->p[i], coef), apply_pitch(map->p[i + 1], coef), map->p[i].pitch);
				draw_segment(env.img, apply_pitch(map->p[i], coef), apply_pitch(map->next->p[i], coef), map->p[i].pitch);
			}
			if ((map->p[i].x < WIDTH || map->p[i].y < HEIGHT)
					&& (map->p[i].x >= 0 || map->p[i].y >= 0))
				draw_segment(env.img, apply_pitch(map->next->p[i], coef), apply_pitch(map->next->p[i + 1], coef), map->p[i].pitch);
			i++;
		}
		if ((map->p[i].x < WIDTH || map->p[i].y < HEIGHT)
				&& (map->p[i].x >= 0 || map->p[i].y >= 0))
			draw_segment(env.img, apply_pitch(map->p[i], coef), apply_pitch(map->next->p[i], coef),map->p[i].pitch);
		i = 0;
		map = map->next;
	}
}

static int open_file(int ac, char **av)
{
	int fd;

	check_params(ac);
	fd = open(av[1], O_RDONLY);
	if (fd == -1)
		catch_errors(1, av[1]);
	return (fd);
}

static int calc_padding(t_map *map, int zoom)
{
	int biggest_line;	
	int lines;
	int padding;
	t_map *index;
	int i;

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
	while (((padding)* lines) < HEIGHT && ((padding) * biggest_line) < WIDTH)
		padding++;
	padding += zoom;
	//padding -= zoom;
	padding /= 1.1;
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
	return (padding);
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
	mlx_put_image_to_window (env.mlx, env.wnd, env.img.img_ptr, 0, 0);
}

int dec_padding(t_global *global, int zoom)
{
	int lines;
	int padding;
	t_map *index;
	int i;

	lines = 0;
	index = global->map;
	padding = 1;
	//zoom /= 1.1;
	erase_map(global->env);
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
	trace_map(global->env, global->map, i, global->map->margin);
	mlx_put_image_to_window(global->env.mlx, global->env.wnd, global->env.img.img_ptr, 0, 0);
	return (padding);
}

int inc_padding(t_global *global, int zoom)
{
	int lines;
	int padding;
	t_map *index;
	int i;

	lines = 0;
	index = global->map;
	padding = 1;
/*	biggest_line = 0;
	while (map)
	{
		if (biggest_line < map->size_line)
			biggest_line = map->size_line;
		map = map->next;
		lines++;
	}
	while (((padding)* lines) < HEIGHT && ((padding) * biggest_line) < WIDTH)
		padding++;
	padding += zoom;
	//padding -= zoom;
	*/
	//zoom /= 1.1;
	erase_map(global->env);
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
	trace_map(global->env, global->map, i, global->map->margin);
	mlx_put_image_to_window(global->env.mlx, global->env.wnd, global->env.img.img_ptr, 0, 0);
	return (padding);
}



void	create_map(t_meta env, t_map *map, int zoom, int i)
{
	int padding;
	(void)zoom;
	(void)padding;
	(void)i;

	padding = calc_padding(map, zoom);
	calc_iso(map);
	//apply_pitch(map, 0);
	decal(map, (ABSOL(map->p[0].x) * 2), 0);
	trace_map(env, map, i, map->margin);
	mlx_put_image_to_window(env.mlx, env.wnd, env.img.img_ptr, 0, 0);
}

void	change_pitch(t_map *map, int coef)
{
	int i;

	i = 0;
	while (map)
	{
		while (map->p[i].next == 1)
		{
			map->p[i].y += (map->p[i].pitch * coef) / map->size_line;
			i++;
		}
		map->p[i].y += ((map->p[i].pitch * coef) / map->size_line);
		//map->p[i].y += coef;
		i = 0;
		map = map->next;
	}
}

void	create_map2(t_global *global,  int zoom, int i, t_coord margin)
{
	static int zoom_maj = 0;
	static int pitch_maj = 0;

	zoom_maj += zoom;
	pitch_maj += i;

	erase_map(global->env);
	//change_padding(global, zoom);
	//calc_iso(map);
	//apply_pitch(map);
	//change_pitch(map, 10);
	decal(global->map, margin.y, margin.x);
	trace_map(global->env, global->map, pitch_maj, margin);
	mlx_put_image_to_window(global->env.mlx, global->env.wnd, global->env.img.img_ptr, 0, 0);
	ft_trace(NULL, "pass");
}


int		my_key_hook(int key_code, t_global *global)
{
	global->map->margin.x = 0;
	global->map->margin.y = 0;
	ft_nbrtrace("key_code", key_code);
	if (key_code == 78)
		create_map2(global, 0, -1, global->map->margin);
	else if (key_code == 69)
		create_map2(global, 0, 1, global->map->margin);
	else if (key_code == 13)
	{
		global->map->margin.y = -7;
		create_map2(global, 0, 0, global->map->margin);
	}
	else if (key_code == 1)
	{
		global->map->margin.y = 7;
		create_map2(global, 0, 0, global->map->margin);
	}
	else if (key_code == 0)
	{
		global->map->margin.x = -7;
		create_map2(global, 0, 0, global->map->margin);
	}
	else if (key_code == 2)
	{
		global->map->margin.x = 7;
		create_map2(global, 0, 0, global->map->margin);
	}
	
	else if (key_code == 126)
	{
		//create_map2(global, 2, 0, global->map->margin);
		inc_padding(global, 2);
	}
	else if (key_code == 125)
	{
		dec_padding(global, 2);
		//create_map2(global, -1, 0, global->map->margin);
	}
	
	else if (key_code == 53)
		exit(0);
	return (0);
}



int		main(int argc, char **argv)
{
	t_global global;
	t_meta	*env;
	int		fd;
	//t_map	*map;
	//int zoom = 5;
	env = &global.env;


	fd = open_file(argc, argv);
	global.map = init_map(fd);
	close(fd);
	env->mlx = mlx_init();
	env->wnd = mlx_new_window(env->mlx, WIDTH, HEIGHT, "Fdf");
	env->img.img_ptr = mlx_new_image(env->mlx, WIDTH, HEIGHT);	
	env->img.img_addr = mlx_get_data_addr(env->img.img_ptr, &(env->img.bits_per_pixel), &(env->img.size_line), &(env->img.endian));
	printf("bits_per_pixel : %d / size_line : %d / endian : %d\n", env->img.bits_per_pixel, env->img.size_line, env->img.endian);

	global.map->margin.x = 100;
	global.map->margin.y = 100;

	ft_nbrtrace("rouge", 0x00FF0000);
	ft_nbrtrace("bleu", 0x000000FF);

	create_map(global.env, global.map, 3, 0);
	ft_nbrtrace("bleu", 0x000000FF);


	mlx_hook(env->wnd, 2, (1L << 0) , &my_key_hook, &global);
	//mlx_expose_hook(env->wnd, )
	mlx_loop(env->mlx);
	return (0);
}
