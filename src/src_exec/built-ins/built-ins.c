/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built-ins.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/14 19:15:09 by pabloglez         #+#    #+#             */
/*   Updated: 2024/11/30 19:48:40 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	is_builtin(const char *cmd)
{
	return (ft_strnstr("echo|cd|pwd|export|unset|env|exit", cmd, 34) != NULL);
}

static int	execute_builtin(t_cmd *cmd, t_minishell *shell)
{
	if (ft_strcmp(cmd->arguments[0], "echo") == 0)
		ft_echo(shell, cmd->arguments);
	else if (ft_strcmp(cmd->arguments[0], "cd") == 0)
		ft_cd(shell, cmd->arguments);
	else if (ft_strcmp(cmd->arguments[0], "pwd") == 0)
		ft_pwd(shell);
	else if (ft_strcmp(cmd->arguments[0], "export") == 0)
		ft_export(shell, cmd->arguments);
	else if (ft_strcmp(cmd->arguments[0], "unset") == 0)
		ft_unset(shell, cmd->arguments);
	else if (ft_strcmp(cmd->arguments[0], "env") == 0)
		ft_env(shell);
	else if (ft_strcmp(cmd->arguments[0], "exit") == 0)
		ft_exit(shell, cmd->arguments);
	else
		return (0);
	return (1);
}

int	handle_builtin(t_cmd *cmd, t_minishell *shell)
{
	if (!is_builtin(cmd->arguments[0]))
		return (0);
	if (handle_redirection(shell, cmd->redir, -1) == 1)
		return (1);
	return (execute_builtin(cmd, shell));
}
