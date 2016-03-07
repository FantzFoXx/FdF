/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   t_map.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: udelorme <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/03/06 05:20:22 by udelorme          #+#    #+#             */
/*   Updated: 2016/03/06 08:12:41 by udelorme         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "libft.h"
#include "t_map.h"
#include "fdf.h"
/*
// remove
static t_coord		*t_coord_new(int x, int y)
{
	t_coord *new;

	new = (t_coord *)malloc(sizeof(t_coord));
	if (new)
	{
		new->x = x;
		new->y = y;
		new->next = NULL;
	}
	return (new);
}

// remove
static void		t_coord_push(t_coord **first, t_coord *new)
{
	t_coord *index;

	index = *first;
	if (!index)
		*first = new;
	else
	{
		while (index->next)
			index = index->next;
		index->next = new;
	}
}
*/
t_map		*t_map_new(char *line, size_t line_nb)
{
	t_map	*new;
	char	**spl;
	int		i;
	size_t	line_size;
	int		padding;

	line_size = ft_wordcount(line, ' ');
	spl = ft_strsplit(line, ' ');
	new = (t_map *)malloc(sizeof(t_map));
	new->size_line = line_size;
	i = -1;
	padding = 10;
	if (new)
	{
		new->p = (t_coord *)malloc(sizeof(t_coord) * line_size + 1);
		i = -1;
		while (spl[++i])
		{
			//ft_trace(NULL, "pass");
			new->p[i].x = i/* * padding*/;
			new->p[i].y = line_nb /** padding*/;
			new->p[i].pitch = ft_atoi(spl[i]);
			//ft_nbrtrace("rea atoi", new->p[i].pitch);
			if (spl[i + 1] != NULL)
				new->p[i].next = 1;
			else
				new->p[i].next = 0;
		}
		new->next = NULL;
	}
	return (new);
}

void		t_map_push(t_map **first, t_map *new)
{
	t_map *index;

	index = *first;
	if (!index)
		*first = new;
	else
	{
		while (index->next)
			index = index->next;
		index->next = new;
	}
}


