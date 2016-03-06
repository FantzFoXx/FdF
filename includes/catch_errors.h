/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   catch_errors.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: udelorme <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/03/06 04:14:01 by udelorme          #+#    #+#             */
/*   Updated: 2016/03/06 05:16:36 by udelorme         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>

int		catch_errors(int err_code, char *err_msg);
void	err_bad_usage(void);
void	check_params(int ac);
