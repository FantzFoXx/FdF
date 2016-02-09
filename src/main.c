#include <unistd.h>
#include "fdf.h"
#include <mlx.h>
#include <math.h>
#include "libft.h"
#include <stdio.h>

/*
   void	trace_droite(void *mlx, void *wnd, t_coord point_a, t_coord point_b)
   {
   t_coord	index;

   index.x = point_a.x;
   index.y = point_a.y;
   if (index.x != point_b.x || index.y != point_b.y)
   {
   index.x = floor((index.x / 2) + (point_b.x / 2));
   index.y = floor((index.y / 2) + (point_b.y / 2));
   printf("x : %f\n", index.x);
   printf("y : %f\n", index.y);
   mlx_pixel_put(mlx, wnd, (int)index.x, (int)index.y, 255);
   ft_trace("rec_b", "pass");
   trace_droite(mlx, wnd, point_a, index);
   if (index.x != point_a.x || index.y != point_a.y)
   {
   ft_trace("rec_a", "pass");
   trace_droite(mlx, wnd, index, point_b);
   }
   }
   }
   */

void	trace_droite(void *mlx, void *wnd, t_coord point_a, t_coord point_b)
{
	float a;
	int i;
	t_coord	index;

	index.x = point_a.x;
	index.y = point_a.y;
	a = (point_b.y - point_a.y) / (point_b.x - point_a.x);

	i = 0;
	while (index.x < point_b.x)
	{
		mlx_pixel_put(mlx, wnd, index.x, (a * i + point_a.y), 255);
		index.x++;
		i++;
	}

}

void	call_trace(void *mlx, void *wnd, t_coord point_a, t_coord point_b)
{
	if (point_a.y < point_b.y)
		trace_droite(mlx, wnd, point_b, point_a);
	else
		trace_droite(mlx, wnd, point_a, point_b);

}

int		main(void)
{
	void	*mlx;
	void	*wnd;
	t_coord	point_a;
	t_coord	point_b;

	point_a.x = 900;
	point_a.y = 0;

	point_b.x = 0;
	point_b.y = 1050;

	mlx = mlx_init();
	wnd = mlx_new_window(mlx, WIDTH, HEIGHT, "Test 1");
	mlx_pixel_put(mlx, wnd, point_a.x, point_a.y, 0x00FF0000);
	mlx_pixel_put(mlx, wnd, point_a.x + 1, point_a.y + 1, 0x00FF0000);
	mlx_pixel_put(mlx, wnd, point_a.x + 1, point_a.y, 0x00FF0000);
	mlx_pixel_put(mlx, wnd, point_a.x, point_a.y + 1, 0x00FF0000);

	mlx_pixel_put(mlx, wnd, point_b.x, point_b.y, 0x00FF0000);
	mlx_pixel_put(mlx, wnd, point_b.x + 1, point_b.y + 1, 0x00FF0000);
	mlx_pixel_put(mlx, wnd, point_b.x, point_b.y + 1, 0x00FF0000);
	mlx_pixel_put(mlx, wnd, point_b.x + 1, point_b.y, 0x00FF0000);

	call_trace(mlx, wnd, point_a, point_b);

	mlx_loop(mlx);
	return (0);
}
