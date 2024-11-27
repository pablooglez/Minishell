/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built-ins.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pablogon <pablogon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/14 19:15:09 by pabloglez         #+#    #+#             */
/*   Updated: 2024/11/27 17:31:42 by pablogon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	handle_builtin(t_cmd *cmd, t_minishell *shell)
{
	if (ft_strnstr("echo|cd|pwd|export|unset|env|exit", cmd->arguments[0], 33))
		handle_redirection(shell, cmd->redir, -1);
	if (ft_strncmp(cmd->arguments[0], "echo", 5) == 0)
	{
		ft_echo(shell, cmd->arguments);
		return (1);
	}
	else if (ft_strncmp(cmd->arguments[0], "cd", 3) == 0)
	{
		ft_cd(shell, cmd->arguments);
		return (1);
	}
	else if (ft_strncmp(cmd->arguments[0], "pwd", 4) == 0)
	{
		ft_pwd(shell);
		return (1);
	}
	else if(ft_strncmp(cmd->arguments[0], "export", 7) == 0)
	{
		ft_export(shell, cmd->arguments);
		return (1);
	}
	else if (ft_strncmp(cmd->arguments[0], "unset", 6) == 0)
	{
		ft_unset(shell, cmd->arguments);
		return (1);
	}
	else if (ft_strncmp(cmd->arguments[0], "env", 4) == 0)
	{
		ft_env(shell);
		return (1);
	}
	else if (ft_strncmp(cmd->arguments[0], "exit", 5) == 0)
	{
		exit_shell(shell);
		return (1);
	}
	return (0);
}

