/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   t_map.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: udelorme <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/03/06 05:21:48 by udelorme          #+#    #+#             */
/*   Updated: 2016/03/10 19:09:15 by udelorme         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef T_MAP_H
# define T_MAP_H

#include "fdf.h"
//#include "t_map.h"

typedef struct	s_coord
{
	double			x;
	double			y;
	int				pitch;
	int				next;
}				t_coord;

typedef struct	s_map
{
	t_coord			*p;
	int				size_line;
	int				nb_line;
	t_coord			margin;
	double			padding;
	int				pitch_maj;
	int				zoom_maj;
	struct s_map	*next;
}				t_map;

typedef struct	s_global
{
	t_meta	env;
	t_map	*map;
	size_t		map_lines;
	size_t		map_col;
	int			high_pitch;
	int			low_pitch;
	int			colors[5];
}				t_global;

void			t_map_push(t_map **first, t_map *new);
t_map			*t_map_new(char *line, size_t line_nb, size_t *size_line, t_global *global);

#endif
