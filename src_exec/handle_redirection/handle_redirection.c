/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_redirection.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albelope <albelope@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/25 17:59:21 by pabloglez         #+#    #+#             */
/*   Updated: 2024/11/20 20:30:52 by albelope         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_redirection(t_cmd *cmd)
{
	t_redir *redir = cmd->redir;												// Inicializa un puntero 'redir' apuntando a la primera redirección en la lista de redirecciones del comando actual.
	int	fd;																		// Declaración de una variable para almacenar el descriptor de archivo que se abrirá para redirección.

	while (redir)																// Itera a través de cada redirección en la lista de redirecciones del comando.
	{
		//printf("[DEBUG]->HANDLE_REDIRECTION->Redir %s\n", redir->file);
		if (redir->type == INFILE)												// Si el tipo de redirección es INFILE (redirección de entrada)...
		{
			//printf("[DEBUG]->HANDLE_REDIRECTION->INFILE\n");
			fd = open(redir->file, O_RDONLY);									// Intenta abrir el archivo en modo lectura.
			//printf("[DEBUG]->HANDLE_REDIRECTION->INFILE->fd: %d\n", fd);
			if (fd == -1)														// Si la apertura falla...
			{
				perror("Error abriendo archivo de entrada");					// Imprime un mensaje de error.
				exit(EXIT_FAILURE);												// Sale del programa con un código de error.
			}
			dup2(fd, STDIN_FILENO);
			//printf("[DEBUG]->HANDLE_REDIRECTION->INFILE->STDIN_FILENO: %d\n", STDIN_FILENO);
			close(fd);															// Cierra el descriptor de archivo, ya que la duplicación fue exitosa.
		}
		else if (redir->type == OUTFILE)										// Si el tipo de redirección es OUTFILE (redirección de salida)...
		{
			//printf("[DEBUG]->HANDLE_REDIRECTION->OUTFILE\n");
			fd = open(redir->file, O_WRONLY | O_CREAT | O_TRUNC, 0644); 		// Abre el archivo en modo escritura, truncándolo o creándolo si no existe.
			//printf("[DEBUG]->HANDLE_REDIRECTION->OUTFILE->fd: %d\n", fd);
			if (fd == -1)														// Si la apertura falla...
			{
				perror("Error abriendo archivo de salida");						// Imprime un mensaje de error.
				exit(EXIT_FAILURE);												// Sale del programa con un código de error.
			}
			dup2(fd, STDOUT_FILENO);
			//printf("[DEBUG]->HANDLE_REDIRECTION->OUTFILE->STDOUT_FILENO: %d\n", STDOUT_FILENO);
			close(fd);															// Cierra el descriptor de archivo, ya que la duplicación fue exitosa.
		}
		else if (redir->type == APPEND)											// Si el tipo de redirección es APPEND (redirección de salida en modo append)...
		{
			fd = open(redir->file, O_WRONLY | O_CREAT | O_APPEND, 0644); 		// Abre el archivo en modo append, creándolo si no existe.
			//printf("[DEBUG]->HANDLE_REDIRECTION->APPEND->fd: %d\n", fd);
			if (fd == -1)														// Si la apertura falla...
			{
				perror("Error abriendo archivo para append");					// Imprime un mensaje de error.
				exit(EXIT_FAILURE);												// Sale del programa con un código de error.
			}
			dup2(fd, STDOUT_FILENO);
			//printf("[DEBUG]->HANDLE_REDIRECTION->APPEND->STDOUT_FILENO: %d\n", STDOUT_FILENO);
			close(fd);															// Cierra el descriptor de archivo, ya que la duplicación fue exitosa.
		}
		//printf("[DEBUG]->HANDLE_REDIRECTION->end STDIN_FILENO: %d\n", STDIN_FILENO);
		redir = redir->next;													// Avanza al siguiente elemento en la lista de redirecciones.
	}	
	//printf("[DEBUG]->HANDLE_REDIRECTION->end2 STDOUT_FILENO: %d\n", STDOUT_FILENO);
	//printf("[DEBUG]->HANDLE_REDIRECTION->end\n");
}
