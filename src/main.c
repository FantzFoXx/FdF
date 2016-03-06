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

static void		img_put_pixel(t_img_prop img, int x, int y, unsigned color)
{
	int	final;
	(void)color;

	x *= (img.bits_per_pixel / 8);
	y *= img.size_line;

	final = x + y;
	img.img_addr[final + 0] = 255;
	img.img_addr[final + 1] = 0;
	img.img_addr[final + 2] = 55;
	img.img_addr[final + 3] = 50;
}

void	draw_segment(t_img_prop img, t_coord *point_a, t_coord *point_b, int color)
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
	img_put_pixel(img, index.x, index.y, color);
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
			img_put_pixel(img, index.x, index.y, color);
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
			img_put_pixel(img, index.x, index.y, color);
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

static void trace_map(t_meta env, t_map *map)
{
	int i;

	i = 0;
	while (map->next)
	{
		ft_trace(NULL, "pass");
		ft_nbrtrace("value", map->p[i].next);
		while (map->p[i].next == 1)
		{
			draw_segment(env.img, &map->p[i], &map->p[i + 1], 255);
			ft_nbrtrace("value point y : ", map->p[i].y);
			ft_nbrtrace("value point x : ", map->p[i].x);
			draw_segment(env.img, &map->p[i], &map->next->p[i], 255);
			i++;
		}
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

int		main(int argc, char **argv)
{
	t_meta	env;
	int		fd;
	t_map	*map;

	fd = open_file(argc, argv);
	map = init_map(fd);
	close(fd);
	env.mlx = mlx_init();
	env.wnd = mlx_new_window(env.mlx, WIDTH, HEIGHT, "Fdf");
	env.img.img_ptr = mlx_new_image(env.mlx, WIDTH, HEIGHT);	
	env.img.img_addr = mlx_get_data_addr(env.img.img_ptr, &(env.img.bits_per_pixel), &(env.img.size_line), &(env.img.endian));
	



	printf("bits_per_pixel : %d / size_line : %d / endian : %d\n", env.img.bits_per_pixel, env.img.size_line, env.img.endian);
	//put_points(map, env);
	trace_map(env, map);

	//draw_segment(env.img, &a, &b, 255);
	mlx_put_image_to_window (env.mlx, env.wnd, env.img.img_ptr, 10, 10 );

	mlx_loop(env.mlx);
	return (0);
}
