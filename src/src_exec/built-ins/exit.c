/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pablogon <pablogon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/28 16:41:21 by pablogon          #+#    #+#             */
/*   Updated: 2024/11/30 17:30:31 by pablogon         ###   ########.fr       */
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

static void	handle_no_args(t_minishell *shell)
{
	if (shell->is_child == false)
		delete_heredoc(shell);
	free_shell(&shell);
	exit(shell->exit_status);
}

void	ft_exit(t_minishell *shell, char **args)
{
	if (!shell->is_child)
		ft_putendl_fd("exit", STDERR_FILENO);
	if (args[1] && args[2])
	{
		ft_putendl_fd("minishell: exit: too many arguments", STDERR_FILENO);
		shell->exit_status = 1;
		return ;
	}
	if (!args[1])
		handle_no_args(shell);
	if (is_numeric(args[1]))
	{
		shell->exit_status = ft_atoi(args[1]) % 256;
		handle_no_args(shell);
	}
	ft_putstr_fd("minishell: exit: ", STDERR_FILENO);
	ft_putstr_fd(args[1], STDERR_FILENO);
	ft_putendl_fd(": numeric argument required", STDERR_FILENO);
	shell->exit_status = 255;
	handle_no_args(shell);
}
