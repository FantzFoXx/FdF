#ifndef FDF_H
# define FDF_H

# define WIDTH 1920
# define HEIGHT 1080
# define ABSOL(x) ((x) < 0 ? - (x) : (x))

typedef struct	s_coord
{
	int		x;
	int	y;
	int	index;
	struct s_coord *right;
	struct s_coord *down;
}				t_coord;

typedef struct	s_pitch_map
{
	char				**line;
	struct	s_pitch_map *next;
}				t_pitch_map;

typedef struct	s_meta
{
	void *mlx;
	void *wnd;
}				t_meta;

#endif
