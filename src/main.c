#include <unistd.h>
#include "t_map.h"
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
#include <math.h>

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

int		calc_rgb(int r, int g, int b)
{
	int ret;

	//ret = 0;
	ret = b;
	ret += g * 0x100;
	ret += r * 0x10000;
	return (ret);
}

int		pitch_color(int pitch, t_global *global)
{
	if (pitch >= ((global->high_pitch * 50) / 100))	
		return (0x00FFFFFF);
	else if (pitch >= ((global->high_pitch * 15) / 100))	
		return (0x006C4500);
	else if (pitch >= ((global->high_pitch * 10) / 100) || pitch > 0)	
		return (0x0083AB00);
	else if (/*pitch >= 0 &&*/ pitch == 0)	
		return (0x00004FC6);
	else if (pitch < 0)
		return (0x00D0B15C);
	return (0x00FFFFFF);
}

void	draw_segment(t_img_prop img, t_coord point_a, t_coord point_b, double pitch, t_global *global)
{
	t_coord d_point;
	t_coord index;
	int	i;
	int	xinc;
	int	yinc;
	int	cumul;



	//ft_trace(NULL, "begin pass");
	index.x = point_a.x;
	index.y = point_a.y;
	d_point.x = point_b.x - point_a.x;
	d_point.y = point_b.y - point_a.y;
	xinc = ( d_point.x > 0 ) ? 1 : -1;
	yinc = ( d_point.y > 0 ) ? 1 : -1;
	d_point.x = ABSOL(d_point.x);
	d_point.y = ABSOL(d_point.y);
	if ((index.x >= 0 && index.y >= 0) && (index.x < WIDTH && index.y < HEIGHT))
		img_put_pixel(img, index.x, index.y, pitch_color(pitch, global));
		//img_put_pixel(img, index.x, index.y, RGB(127.5 * (cos(pitch) + 1), 127.5 * (sin(pitch) + 1), 127.5 * (1 - cos(pitch))));
		//img_put_pixel(img, index.x, index.y, calc_rgb((cos(pitch) + 1), (sin(pitch) + 1), (1 - cos(pitch))));
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
				img_put_pixel(img, index.x, index.y, pitch_color(pitch, global));
				//img_put_pixel(img, index.x, index.y, RGB(127.5 * (cos(pitch) + 1), 127.5 * (sin(pitch) + 1), 127.5 * (1 - cos(pitch))));
		} 
	}
	else 
	{
		cumul = d_point.y / 2;
		for (i = 1 ; i <= d_point.y ; i++) 
		{
			//ft_nbrtrace("for", d_point.y);
			index.y += yinc;
			cumul += d_point.x;
			if (cumul >= d_point.y) 
			{
				cumul -= d_point.y;
				index.x += xinc; 
			}
			if ((index.x >= 0 && index.y >= 0) && (index.x < WIDTH && index.y < HEIGHT))
				//img_put_pixel(img, index.x, index.y, color);
				img_put_pixel(img, index.x, index.y, pitch_color(pitch, global));
				//img_put_pixel(img, index.x, index.y, RGB(127.5 * (cos(pitch) + 1), 127.5 * (sin(pitch) + 1), 127.5 * (1 - cos(pitch))));
		}
	}
	//ft_trace(NULL, "end pass");
}

static t_map	*init_map(int fd, char *filename, t_global *global)
{
	char *line;
	t_map *map;
	t_map *index;
	int		gnl_ret;

	map = NULL;
	index = map;
	gnl_ret = 0;
	global->map_col = 0;
	global->map_lines = 0;
	global->high_pitch = 0;
	global->low_pitch = 0;
	while ((gnl_ret = get_next_line(fd, &line)) && gnl_ret > 0)
	{
		t_map_push(&map, t_map_new(line, global->map_lines, &global->map_col, global));
		free(line);
		global->map_lines += 1;
	}
	if (gnl_ret == -1 || global->map_lines == 0)
		catch_errors(3, filename);
	return (map);
}
/*
   static void	calc_iso(t_map *map, t_global *global)
   {
   (void)global;
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

int		get_iso_decal(t_global *global)
{
	t_map *map;

	map = global->map;
	while (map->next)
		map = map->next;
	return (ABSOL(map->p[0].x));
}

static void decal(t_map *map, int value_up, int value_down)
{
	int i;
	t_map *index;

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

void	calc_iso(t_map *map, t_global *global)
{
	t_map *bak;
	int i;
	int x_bak;
	int y_bak;

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

static t_coord apply_pitch(t_coord p, int coef, t_global *global)
{
	t_coord pitched;

	pitched.x = p.x;
	pitched.y = p.y - ((p.pitch * ((coef * (global->map->padding)))) * 0.01);
	return (pitched);
}

static void trace_map(t_meta env, t_map *map, int coef, t_coord margin, t_global *global)
{
	int i;
	(void)margin;

	i = 0;
	if (map && !map->next)
		while (map->p[i].next == 1)
		{
			if ((map->p[i].x < WIDTH || map->p[i].y < HEIGHT)
					&& (map->p[i].x >= 0 || map->p[i].y >= 0))
				draw_segment(env.img, apply_pitch(map->p[i], coef, global), apply_pitch(map->p[i + 1], coef, global), map->p[i].pitch, global);
			//if ((map->p[i].x < WIDTH || map->p[i].y < HEIGHT)
			//		&& (map->p[i].x >= 0 || map->p[i].y >= 0))
			//	draw_segment(env.img, apply_pitch(map->next->p[i], coef, global), apply_pitch(map->next->p[i + 1], coef, global), map->p[i].pitch);
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
					draw_segment(env.img, apply_pitch(map->p[i], coef, global), apply_pitch(map->p[i + 1], coef, global), map->p[i].pitch, global);
					draw_segment(env.img, apply_pitch(map->p[i], coef, global), apply_pitch(map->next->p[i], coef, global), map->p[i].pitch, global);
				}
				if ((map->p[i].x < WIDTH || map->p[i].y < HEIGHT)
						&& (map->p[i].x >= 0 || map->p[i].y >= 0))
					draw_segment(env.img, apply_pitch(map->next->p[i], coef, global), apply_pitch(map->next->p[i + 1], coef, global), map->p[i].pitch, global);
				i++;
			}
			if ((map->p[i].x < WIDTH || map->p[i].y < HEIGHT)
					&& (map->p[i].x >= 0 || map->p[i].y >= 0))
				draw_segment(env.img, apply_pitch(map->p[i], coef, global), apply_pitch(map->next->p[i], coef, global),map->p[i].pitch, global);
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

double dec_padding(t_global *global, int zoom)
{
	int lines;
	double padding;
	t_map *index;
	int i;

	lines = 0;
	index = global->map;
	padding = global->map->padding / zoom;
	//zoom /= 1.1;
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
	decal(global->map, (HEIGHT / 4), (WIDTH / 4));
	return (padding);
}

double inc_padding(t_global *global, int zoom)
{
	int lines;
	double padding;
	t_map *index;
	int i;

	lines = 0;
	index = global->map;
	padding = global->map->padding * zoom;
	//zoom /= 1.1;
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
	decal(global->map, -(HEIGHT / 2), -(WIDTH / 2));
	return (padding);
}



void	create_map(t_global *global)
{

	ft_trace(">>> Calculating padding" ,"...");
	global->map->padding = calc_padding(global->map, global->map->zoom_maj);
	ft_putendl("Done.");
	ft_trace(">>> Calculating ISO" ,"...");
	calc_iso(global->map, global);
	ft_putendl("Done.");
	//apply_pitch(map, 0);
	global->map->margin.y = 20;
	global->map->margin.x = 20;
	ft_trace(">>> Correcting ISO placement" ,"...");
	decal(global->map, global->map->margin.y, global->map->margin.x);
	ft_putendl("Done.");
	ft_trace(">>> Tracing map" ,"...");
	trace_map(global->env, global->map, global->map->pitch_maj, global->map->margin, global);
	ft_putendl("Done.");
	ft_trace(">>> Displaying map" ,"...");
	mlx_put_image_to_window(global->env.mlx, global->env.wnd, global->env.img.img_ptr, 0, 0);
	ft_putendl("Done.");
}

void	change_pitch(t_global *global, int coef)
{
	int i;

	i = 0;
	while (global->map)
	{
		while (global->map->p[i].next == 1)
		{
			global->map->p[i].y += (global->map->p[i].pitch * coef) / global->map->padding;
			i++;
		}
		global->map->p[i].y += ((global->map->p[i].pitch * coef) / global->map->padding);
		//map->p[i].y += coef;
		i = 0;
		global->map = global->map->next;
	}
}

void	reload_map(t_global *global,  int zoom, int i, t_coord margin)
{
	(void)margin;
	global->map->zoom_maj += zoom;
	global->map->pitch_maj += i;

	//calc_iso(global->map);
	//change_pitch(global, 1);
	//decal(global->map, margin.y, margin.x);
	ft_trace(">>> Erasing old map" ,"...");
	erase_map(global->env);
	ft_putendl("Done.");
	ft_trace(">>> Tracing map" ,"...");
	trace_map(global->env, global->map, global->map->pitch_maj, margin, global);
	ft_putendl("Done.");
	ft_trace(">>> Displaying map" ,"...");
	mlx_put_image_to_window(global->env.mlx, global->env.wnd, global->env.img.img_ptr, 0, 0);
	ft_putendl("Done.");
	ft_nbrtrace("Padding value", global->map->padding);
}


int		my_key_hook(int key_code, t_global *global)
{
	ft_nbrtrace("key_code", key_code);
	ft_trace(">>> Recalculating data" ,"...");
	global->map->margin.x = 0;
	global->map->margin.y = 0;
	if (key_code == 125)
		reload_map(global, 0, -1, global->map->margin);
	else if (key_code == 126)
		reload_map(global, 0, 1, global->map->margin);
	else if (key_code == 13)
	{
		global->map->margin.y = -10;
		decal(global->map, global->map->margin.y, global->map->margin.x);
		reload_map(global, 0, 0, global->map->margin);
	}
	else if (key_code == 1)
	{
		global->map->margin.y = 10;
		decal(global->map, global->map->margin.y, global->map->margin.x);
		reload_map(global, 0, 0, global->map->margin);
	}
	else if (key_code == 0)
	{
		global->map->margin.x = -10;
		decal(global->map, global->map->margin.y, global->map->margin.x);
		reload_map(global, 0, 0, global->map->margin);
	}
	else if (key_code == 2)
	{
		global->map->margin.x = 10;
		decal(global->map, global->map->margin.y, global->map->margin.x);
		reload_map(global, 0, 0, global->map->margin);
	}

	else if (key_code == 69)
	{
		global->map->padding = inc_padding(global, 2);
		reload_map(global, 0, 0, global->map->margin);
	}
	else if (key_code == 78)
	{
		global->map->padding = dec_padding(global, 2);
		reload_map(global, 0, 0, global->map->margin);
	}

	else if (key_code == 53)
		exit(0);
	return (0);
}

void	get_user_params(int ac, char **av, t_global *global)
{
	if (ac == 4)
	{
		global->map->pitch_maj = ft_atoi(av[2]);
		global->map->zoom_maj = ft_atoi(av[3]);
		ft_nbrtrace("pitch_maj", global->map->pitch_maj);
		ft_nbrtrace("zoom_maj", global->map->zoom_maj);
		if (global->map->zoom_maj == 0)
			global->map->zoom_maj = 1;
	}
	else
	{
		global->map->pitch_maj = 2;
		global->map->zoom_maj = 1;
	}
}

int		main(int argc, char **argv)
{
	t_global global;
	t_meta	*env;
	int		fd;
	env = &global.env;

	ft_trace(">>> Parsing map" ,"...");
	fd = open_file(argc, argv);
	global.map = init_map(fd, argv[1], &global);
	close(fd);
	ft_putendl("Done.");
	ft_trace(">>> Init mlx" ,"...");
	env->mlx = mlx_init();
	ft_putendl("Done.");
	ft_trace(">>> Loading window" ,"...");
	env->wnd = mlx_new_window(env->mlx, WIDTH, HEIGHT, "Fdf");
	ft_putendl("Done.");
	ft_trace(">>> Loading memory image" ,"...");
	env->img.img_ptr = mlx_new_image(env->mlx, WIDTH, HEIGHT);	
	env->img.img_addr = mlx_get_data_addr(env->img.img_ptr, &(env->img.bits_per_pixel), &(env->img.size_line), &(env->img.endian));
	printf("bits_per_pixel : %d / size_line : %d / endian : %d\n", env->img.bits_per_pixel, env->img.size_line, env->img.endian);


	ft_putendl("Done.");
	ft_trace(">>> Charging user preferences" ,"...");
	get_user_params(argc, argv, &global);
	//global.map->pitch_maj = 2;
	//global.map->zoom_maj = 2;

	ft_trace(">>> In create_map" ,"...");
	create_map(&global);
	ft_putendl("Evrything done.");


	mlx_hook(env->wnd, 2, (1L << 0) , &my_key_hook, &global);
	mlx_loop(env->mlx);
	return (0);
}
