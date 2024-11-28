/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pablogon <pablogon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/28 16:41:21 by pablogon          #+#    #+#             */
/*   Updated: 2024/11/28 17:14:14 by pablogon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	is_numeric(const char *str)
{
	int	i;

	i = 0;

	if (str[i] == '+' || str[i] == '-')
		i++;
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (0);
		i++;
	}
	return (1);
}

void	ft_exit(t_minishell *shell, char **args)
{
	int	exit_code;

	if (shell->running)
		ft_putendl_fd("exit", STDERR_FILENO);
	if (args[2])
	{
		ft_putendl_fd("minishell: exit: too many arguments", STDERR_FILENO);
		shell->exit_status = 1;
		return ;
	}
	if (!args[1])
		exit(shell->exit_status);
	if (is_numeric(args[1]))
	{
		exit_code = ft_atoi(args[1]) % 256;
		free_shell(&shell);
		exit(exit_code);
	}
	ft_putstr_fd("minishell: exit: ", STDERR_FILENO);
	ft_putstr_fd(args[1], STDERR_FILENO);
	ft_putendl_fd(": numeric argument required", STDERR_FILENO);
	free_shell(&shell);
	exit(255);
}


