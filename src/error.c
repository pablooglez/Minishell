/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pablogon <pablogon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/01 19:55:18 by pablogon          #+#    #+#             */
/*   Updated: 2024/11/29 20:49:58 by pablogon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	built_ins(int code, char *value)
{
	if (code == MSG && value)
		write(2, value, ft_strlen(value));
	if (code == CD_NOT_FOUND && value)
	{
		write(2, "cd: ", 4);
		write(2, value, ft_strlen(value));
		write(2, ": No such file or directory\n", 29);
	}
}

static void	handle_error(int code, const char *value)
{
	if (code == SYNTAX_ERROR)
	{
		write(2, "Minishell: syntax error near unexpected token `", 47);
		if (value)
			write(2, value, ft_strlen(value));
		write(2, "'\n", 2);
		exit(2);
	}
	else if (code == PERMISSION_DENIED)
	{
		write(2, "Minishell: permission denied: ", 31);
		if (value)
			write(2, value, ft_strlen(value));
		write(2, "\n", 1);
		exit(126);
	}
}

static void	fatal(int code, char *value)
{
	if (code == MEMORY_ERROR)
	{
		write(2, "Minishell: error: Memory allocation failed\n", 43);
		exit(EXIT_FAILURE);
	}
	else if (code == FD_NOT_FOUND)
	{
		write(2, "Minishell: No such file or directory\n", 38);
		exit(129);
	}
	else if (code == CMD_NOT_FOUND)
	{
		write(2, "Minishell: command not found: ", 30);
		if (value)
			write(2, value, ft_strlen(value));
		write(2, "\n", 1);
		exit(127);
	}
	else
		handle_error(code, value);
}

int	ft_error(t_minishell *shell, int code, char *value, int should_exit)
{
	fatal(code, value);
	built_ins(code, value);
	if (code != MSG && value)
		free(value);
	if (should_exit)
		exit(shell->exit_status);
	return (1);
}
