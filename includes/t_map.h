/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   t_map.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: udelorme <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/03/06 05:21:48 by udelorme          #+#    #+#             */
/*   Updated: 2016/03/07 20:03:35 by udelorme         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef T_MAP_H
# define T_MAP_H

typedef struct	s_coord
{
	double				x;
	double				y;
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
	struct	s_map	*next;
}				t_map;

t_map			*t_map_new(char *line, size_t line_nb, size_t *size_line);
void			t_map_push(t_map **first, t_map *new);

#endif
