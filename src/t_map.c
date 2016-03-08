/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   t_map.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: udelorme <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/03/06 05:20:22 by udelorme          #+#    #+#             */
/*   Updated: 2016/03/07 12:30:52 by udelorme         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "libft.h"
#include "t_map.h"
#include "fdf.h"
#include "catch_errors.h"

t_map		*t_map_new(char *line, size_t line_nb, size_t *size_line)
{
	t_map	*new;
	char	**spl;
	int		i;
	size_t	line_size;
	int		padding;

	line_size = ft_wordcount(line, ' ');
	if (line_size < 1 || (line_size != *size_line && line_nb > 0))
		catch_errors(2, NULL);
	*size_line = line_size;
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
				new->p[i].x = i/* * padding*/;
				new->p[i].y = line_nb /** padding*/;
				new->p[i].pitch = ft_atoi(spl[i]);
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


