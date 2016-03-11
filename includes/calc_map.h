/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   calc_map.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: udelorme <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/03/11 11:26:06 by udelorme          #+#    #+#             */
/*   Updated: 2016/03/11 11:53:47 by udelorme         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CALC_MAP_H
# define CALC_MAP_H

#include "t_map.h"
#include "fdf.h"

void		calc_iso(t_map *map, t_global *global);
t_coord 	apply_pitch(t_coord p, int coef, t_global *global);
int 		calc_padding(t_map *map, int zoom);
double		dec_padding(t_global *global, int zoom, int x, int y);
double		inc_padding(t_global *global, int zoom, int x, int y);
int			get_iso_decal(t_global *global);
void 		decal(t_map *map, int value_up, int value_down);
void		change_pitch(t_global *global, int coef);
void		up_tilt_map(t_global *global);
void		down_tilt_map(t_global *global);
void		reload_map(t_global *global,  int zoom, int i, t_coord margin);

#endif
