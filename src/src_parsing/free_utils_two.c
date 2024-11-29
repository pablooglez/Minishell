/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_utils_two.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albelope <albelope@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/29 23:02:16 by albelope          #+#    #+#             */
/*   Updated: 2024/11/29 23:03:36 by albelope         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_error(const char *msg)
{
	if (msg)
		write(2, msg, ft_strlen(msg));
}

int	print_error_and_return(const char *msg)
{
	if (msg)
		write(2, msg, ft_strlen(msg));
	return (-1);
}

void	print_error_and_exit(const char *msg, int exit_code)
{
	if (msg)
		write(2, msg, ft_strlen(msg));
	exit(exit_code);
}

int	error_handler(const char *msg, int exit_code)
{
	if (msg)
		write(2, msg, ft_strlen(msg));
	if (exit_code >= 0)
		exit(exit_code);
	return (exit_code);
}
