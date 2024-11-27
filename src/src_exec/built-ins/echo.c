/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pablogon <pablogon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/27 17:35:48 by pablogon          #+#    #+#             */
/*   Updated: 2024/11/27 17:39:47 by pablogon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_echo(t_minishell *shell, char **arg)
{
	bool	newline_flag;
	int		i;

	(void)shell;
	newline_flag = true;
	i = 1;
	if (arg[1] && ft_strncmp(arg[1], "-n", 3) == 0)
	{
		newline_flag = false;
		i = 2;
	}
	while (arg[i])
	{
		printf("%s", arg[i]);
		i++;
		if (arg[i])
			printf(" ");
	}
	if (newline_flag)
		printf("\n");
}
