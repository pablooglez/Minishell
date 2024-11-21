/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_redirection.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albelope <albelope@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/25 17:59:21 by pabloglez         #+#    #+#             */
/*   Updated: 2024/11/20 20:34:16 by albelope         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	handle_pipe(t_cmd *cmd)
{
	cmd->pipe[0] = -1;
	cmd->pipe[1] = -1;
	cmd->intfd = -1;
	if (cmd->type == PIPE)											// Comprueba si el siguiente comando en la lista es un pipe
	{
		if (pipe(cmd->pipe) == -1)												// Crea una tubería. Si falla, imprime un error y sale del programa.
			return (1);
		cmd->intfd = cmd->pipe[0];
	}
	cmd->pipe[0] = -1;
	if (cmd->prev && cmd->prev->type == PIPE)											// Comprueba si el siguiente comando en la lista es un pipe
		cmd->pipe[0] = cmd->prev->intfd;
	return (0);
}

void	handle_redirection(t_minishell *shell, t_redir *redir, int fd)
{
	while (redir)																// Itera a través de cada redirección en la lista de redirecciones del comando.
	{
		if (redir->type == INFILE)												// Si el tipo de redirección es INFILE (redirección de entrada)...
		{
			fd = open(redir->file, O_RDONLY);									// Intenta abrir el archivo en modo lectura.
			if (fd == -1)														// Si la apertura falla...
				ft_error(shell, MEMORY_ERROR, NULL, 1);
			safe_dup2(fd, STDIN_FILENO);
		}
		else if (redir->type == OUTFILE)										// Si el tipo de redirección es OUTFILE (redirección de salida)...
		{
			fd = open(redir->file, O_WRONLY | O_CREAT | O_TRUNC, 0644); 		// Abre el archivo en modo escritura, truncándolo o creándolo si no existe.
			if (fd == -1)														// Si la apertura falla...
				ft_error(shell, MEMORY_ERROR, NULL, 1);
			safe_dup2(fd, STDOUT_FILENO);
		}
		else if (redir->type == APPEND)											// Si el tipo de redirección es APPEND (redirección de salida en modo append)...
		{
			fd = open(redir->file, O_WRONLY | O_CREAT | O_APPEND, 0644); 		// Abre el archivo en modo append, creándolo si no existe.
			if (fd == -1)														// Si la apertura falla...
				ft_error(shell, MEMORY_ERROR, NULL, 1);
			safe_dup2(fd, STDOUT_FILENO);
		}
		redir = redir->next;													// Avanza al siguiente elemento en la lista de redirecciones.
	}	
}

void	safe_dup2(int fd1, int fd2)
{
	if (fd1 == -1)
		return;
	if (dup2(fd1, fd2) == -1)
		exit(1);
	close(fd1);
}

void	safe_close(int fd)
{
	if (fd != -1)
		close(fd);
}
