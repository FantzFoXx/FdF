/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   t_map.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: udelorme <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/03/06 05:20:22 by udelorme          #+#    #+#             */
/*   Updated: 2016/03/15 11:25:47 by udelorme         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "libft.h"
#include "t_map.h"
#include "catch_errors.h"

void	init_t_map(t_map *new, t_global *global, size_t line_nb, char **spl)
{
	int i;

	i = -1;
	while (spl[++i])
	{
		new->p[i].x = i;
		new->p[i].y = line_nb;
		new->p[i].pitch = ft_atoi(spl[i]);
		if (new->p[i].pitch > 0 && new->p[i].pitch > global->high_pitch)
			global->high_pitch = new->p[i].pitch;
		else if (new->p[i].pitch <= 0
				&& new->p[i].pitch < global->high_pitch)
			global->low_pitch = new->p[i].pitch;
		if (spl[i + 1] != NULL)
			new->p[i].next = 1;
		else
			new->p[i].next = 0;
	}
}

t_map	*t_map_new(char *line, size_t line_nb, size_t *size_line
		, t_global *global)
{
	t_map	*new;
	char	**spl;
	size_t	line_size;

	line_size = ft_wordcount(line, ' ');
	if (line_size < 1 || (line_size != *size_line && line_nb > 0))
		catch_errors(2, NULL);
	*size_line = line_size;
	spl = ft_strsplit(line, ' ');
	new = (t_map *)malloc(sizeof(t_map));
	new->size_line = line_size;
	if (new)
	{
		new->p = (t_coord *)malloc(sizeof(t_coord) * line_size + 1);
		init_t_map(new, global, line_nb, spl);
		new->next = NULL;
	}
	return (new);
}

void	t_map_push(t_map **first, t_map *new)
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

int		open_file(int ac, char **av)
{
	int fd;

	check_params(ac);
	fd = open(av[1], O_RDONLY);
	if (fd == -1)
		catch_errors(1, av[1]);
	return (fd);
}

t_map	*init_map(int fd, char *filename, t_global *global)
{
	char	*line;
	t_map	*map;
	t_map	*index;
	int		gnl_ret;

	map = NULL;
	index = map;
	gnl_ret = 0;
	global->map_col = 0;
	global->map_lines = 0;
	global->high_pitch = 0;
	global->low_pitch = 0;
	while ((gnl_ret = get_next_line(fd, &line)) && gnl_ret > 0)
	{
		t_map_push(&map, t_map_new(line, global->map_lines
					, &global->map_col, global));
		free(line);
		global->map_lines += 1;
	}
	if (gnl_ret == -1)
		catch_errors(1, filename);
	if (global->map_lines == 0)
		catch_errors(2, filename);
	global->env.map_name = filename;
	return (map);
}
