/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pablogon <pablogon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/01 19:55:18 by pablogon          #+#    #+#             */
/*   Updated: 2024/11/30 20:38:00 by pablogon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	printer(const char *s1, const char *s2, const char *s3)
{
	char	*tmp;

	tmp = NULL;
	if (s1 && s2 && !s3)
		tmp = ft_strjoin(s1, s2);
	else if (s1 && s2 && s3)
		tmp = ft_strjoin2(s1, s2, s3);
	if (tmp)
	{
		write(2, tmp, ft_strlen(tmp));
		free(tmp);
	}
}

static void	built_ins(int code, char *value)
{
	if (code == MSG && value)
		write(2, value, ft_strlen(value));
	if (code == CD_NOT_FOUND && value)
		printer("cd: ", value, ": No such file or directory\n");
}

static void	handle_error(int code, const char *value)
{
	if (code == SYNTAX_ERROR)
	{
		printer("Minishell: syntax error near unexpected token `",
			value, "\n");
		exit(2);
	}
	else if (code == PERMISSION_DENIED)
	{
		printer("Minishell: permission denied: ", value, "\n");
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
		write(2, "Minishell: No such file or directory\n", 38);
	else if (code == CMD_NOT_FOUND)
	{
		printer("Minishell: command not found: ", value, "\n");
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
	if (code == FD_NOT_FOUND)
		shell->exit_status = 1;
	return (1);
}
