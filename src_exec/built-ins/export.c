/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pabloglez <pabloglez@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/14 20:00:11 by pabloglez         #+#    #+#             */
/*   Updated: 2024/10/17 21:04:48 by pabloglez        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_export(t_minishell *shell, char **arg)
{
	int	i;

	i = 1;
	
	if (!arg[1])
	{
		print_env_vars(shell->env_vars);
		return;
	}

	while (arg[i])
	{
		char *equal_sign = ft_strchr(arg[i], "=");

		if (equal_sign)
		{
			
		}
	}
}