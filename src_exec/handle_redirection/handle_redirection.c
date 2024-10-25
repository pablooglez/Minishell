/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_redirection.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pabloglez <pabloglez@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/25 17:59:21 by pabloglez         #+#    #+#             */
/*   Updated: 2024/10/25 20:44:17 by pabloglez        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_redirection(t_cmd *cmd)
{
	t_redir *redir = cmd->redir;
	int	fd;

	while (redir)
	{
		if (redir->type == INFILE)
		{
			fd = open(redir->file, O_RDONLY);
			if (fd == -1)
			{
				perror("Error abriendo archivo de entrada");
				exit(EXIT_FAILURE);
			}
			if (dup2(fd, STDIN_FILENO) == -1)
			{
				perror("Error en redirección de entrada");
				close(fd);
				exit(EXIT_FAILURE);
			}
			close(fd);
		}
		else if (redir->type == OUTFILE)
		{
			fd = open(redir->file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
			if (fd == -1)
			{
				perror("Error abriendo archivo de salida");
				exit(EXIT_FAILURE);
			}
			if (dup2(fd, STDOUT_FILENO) == -1)
			{
				perror("Error en redirección de salida");
				close(fd);
				exit(EXIT_FAILURE);
			}
			close(fd);
		}
		else if (redir->type == APPEND)
		{
			fd = open(redir->file, O_WRONLY | O_CREAT | O_APPEND, 0644);
			if (fd == -1)
			{
				perror("Error abriendo archivo para append");
				exit(EXIT_FAILURE);
			}
			if (dup2(fd, STDOUT_FILENO) == -1)
			{
				perror("Error en redirección de salida(append)");
				close(fd);
				exit(EXIT_FAILURE);
			}
			close(fd);
		}
		redir = redir->next;
	}
}