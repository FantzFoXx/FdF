/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_map.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: udelorme <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/03/11 11:16:26 by udelorme          #+#    #+#             */
/*   Updated: 2016/03/11 15:13:02 by udelorme         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PRINT_MAP_H
# define PRINT_MAP_H

#include "t_map.h"
#include "fdf.h"
#include <stdlib.h>

void	img_put_pixel(t_img_prop img, int x, int y, int color);
void	draw_segment(t_img_prop img, t_coord point_a, t_coord point_b, double pitch, t_global *global);
void	trace_map(t_meta env, t_map *map, int coef, t_coord margin, t_global *global);
int		calc_rgb(int r, int g, int b);
int		pitch_color(int pitch, t_global *global);
void	erase_map(t_meta env);

#endif
