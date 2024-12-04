/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pablogon <pablogon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/25 17:59:21 by pabloglez         #+#    #+#             */
/*   Updated: 2024/11/29 20:44:51 by pablogon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	handle_pipe(t_cmd *cmd)
{
	cmd->pipe[0] = -1;
	cmd->pipe[1] = -1;
	cmd->intfd = -1;
	if (cmd->type == PIPE)
	{
		if (pipe(cmd->pipe) == -1)
			return (1);
		cmd->intfd = cmd->pipe[0];
	}
	cmd->pipe[0] = -1;
	if (cmd->prev && cmd->prev->type == PIPE)
		cmd->pipe[0] = cmd->prev->intfd;
	return (0);
}

static int	handle_infile(t_minishell *shell, t_redir *redir)
{
	int	fd;

	fd = open(redir->file, O_RDONLY);
	if (fd == -1)
		return (ft_error(shell, FD_NOT_FOUND, NULL, 0));
	safe_dup2(fd, STDIN_FILENO);
	return (0);
}

int	handle_redirection(t_minishell *shell, t_redir *redir, int fd)
{
	while (redir)
	{
		if (redir->type == INFILE)
		{
			if (handle_infile(shell, redir))
				return (1);
		}
		else if (redir->type == OUTFILE)
		{
			fd = open(redir->file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
			if (fd == -1)
				return (ft_error(shell, FD_NOT_FOUND, NULL, 0));
			safe_dup2(fd, STDOUT_FILENO);
		}
		else if (redir->type == APPEND)
		{
			fd = open(redir->file, O_WRONLY | O_CREAT | O_APPEND, 0644);
			if (fd == -1)
				return (ft_error(shell, FD_NOT_FOUND, NULL, 0));
			safe_dup2(fd, STDOUT_FILENO);
		}
		redir = redir->next;
	}
	return (0);
}

void	safe_dup2(int fd1, int fd2)
{
	if (fd1 == -1)
		return ;
	if (dup2(fd1, fd2) == -1)
		exit(1);
	close(fd1);
}

void	safe_close(int fd)
{
	if (fd != -1)
		close(fd);
}
