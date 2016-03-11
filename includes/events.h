/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   events.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: udelorme <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/03/11 11:34:53 by udelorme          #+#    #+#             */
/*   Updated: 2016/03/11 14:45:19 by udelorme         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EVENTS_H
# define EVENTS_H

#include "events.h"
#include "fdf.h"
#include "t_map.h"

int		my_mouse_hook(int button, int x, int y, t_global *global);
int		my_key_hook(int key_code, t_global *global);
void	print_infos(t_global *global);

#endif
