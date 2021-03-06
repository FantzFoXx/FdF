/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   catch_errors.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: udelorme <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/03/06 04:13:44 by udelorme          #+#    #+#             */
/*   Updated: 2016/03/15 13:44:17 by udelorme         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "catch_errors.h"
#include "libft.h"
#include <stdio.h>
#include <errno.h>

void	catch_errors(int err_code, char *err_msg)
{
	if (err_code == 1)
	{
		if (errno == 21)
		{
			ft_putstr(err_msg);
			ft_putendl(" is a directory");
		}
		else if (errno == 2)
		{
			ft_putstr("No file ");
			ft_putendl(err_msg);
		}
		else if (errno == 13)
		{
			ft_putstr("Permission denied for ");
			ft_putendl(err_msg);
		}
	}
	else if (err_code == 2)
		ft_putendl("Wrong file data");
	exit(1);
}

void	err_bad_usage(void)
{
	ft_putendl("Usage : ./fdf <filename>");
	exit(1);
}

void	check_params(int ac)
{
	if (ac != 2)
		err_bad_usage();
}
