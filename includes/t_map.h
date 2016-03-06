/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   t_map.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: udelorme <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/03/06 05:21:48 by udelorme          #+#    #+#             */
/*   Updated: 2016/03/06 07:16:46 by udelorme         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef T_MAP_H
# define T_MAP_H

#include "fdf.h"

typedef struct	s_map
{
	//char			**line;
	t_coord			*p;
	struct	s_map	*next;
}				t_map;

t_map			*t_map_new(char *line, size_t line_nb);
void			t_map_push(t_map **first, t_map *new);

#endif
