/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fdf.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: udelorme <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/03/06 06:43:46 by udelorme          #+#    #+#             */
/*   Updated: 2016/03/14 18:13:49 by udelorme         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FDF_H
# define FDF_H

# define WIDTH 2500
# define HEIGHT 1400
# define ABSOL(x) ((x) < 0 ? - (x) : (x))
# define RGB(r, g, b)(256 * 256 * (int)(r) + 256 * (int)(g) + (int)(b))

typedef struct	s_img_prop
{
	int		xinc;
	int		yinc;
	void	*img_ptr;
	char	*img_addr;
	int		bits_per_pixel;
	int		size_line;
	int		endian;
}				t_img_prop;

typedef struct	s_meta
{
	void		*mlx;
	void		*wnd;
	t_img_prop	img;
	char		*map_name;
}				t_meta;
#endif
