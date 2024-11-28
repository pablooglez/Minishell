/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built-ins.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pablogon <pablogon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/14 19:15:09 by pabloglez         #+#    #+#             */
/*   Updated: 2024/11/28 17:59:01 by pablogon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	handle_builtin(t_cmd *cmd, t_minishell *shell)
{
	if (ft_strnstr("echo|cd|pwd|export|unset|env|exit", cmd->arguments[0], 34))
		handle_redirection(shell, cmd->redir, -1);
	if (ft_strcmp(cmd->arguments[0], "echo") == 0)
	{
		ft_echo(cmd->arguments);
		return (1);
	}
	else if (ft_strcmp(cmd->arguments[0], "cd") == 0)
	{
		ft_cd(shell, cmd->arguments);
		return (1);
	}
	else if (ft_strcmp(cmd->arguments[0], "pwd") == 0)
	{
		ft_pwd(shell);
		return (1);
	}
	else if (ft_strcmp(cmd->arguments[0], "export") == 0)
	{
		ft_export(shell, cmd->arguments);
		return (1);
	}
	else if (ft_strcmp(cmd->arguments[0], "unset") == 0)
	{
		ft_unset(shell, cmd->arguments);
		return (1);
	}
	else if (ft_strcmp(cmd->arguments[0], "env") == 0)
	{
		ft_env(shell);
		return (1);
	}
	else if (ft_strcmp(cmd->arguments[0], "exit") == 0)
	{
		ft_exit(shell, cmd->arguments);
		return (1);
	}
	return (0);
}

