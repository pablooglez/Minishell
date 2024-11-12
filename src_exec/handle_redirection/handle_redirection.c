/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_redirection.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pabloglez <pabloglez@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/25 17:59:21 by pabloglez         #+#    #+#             */
/*   Updated: 2024/11/12 20:59:18 by pabloglez        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_redirection(t_cmd *cmd)
{
	t_redir *redir = cmd->redir;												// Inicializa un puntero 'redir' apuntando a la primera redirección en la lista de redirecciones del comando actual.
	int	fd;																		// Declaración de una variable para almacenar el descriptor de archivo que se abrirá para redirección.

	while (redir)																// Itera a través de cada redirección en la lista de redirecciones del comando.
	{
		if (redir->type == INFILE)												// Si el tipo de redirección es INFILE (redirección de entrada)...
		{
			fd = open(redir->file, O_RDONLY);									// Intenta abrir el archivo en modo lectura.
			if (fd == -1)														// Si la apertura falla...
			{
				perror("Error abriendo archivo de entrada");					// Imprime un mensaje de error.
				exit(EXIT_FAILURE);												// Sale del programa con un código de error.
			}
			close(fd);															// Cierra el descriptor de archivo, ya que la duplicación fue exitosa.
			cmd->intfd=fd;
		}
		else if (redir->type == OUTFILE)										// Si el tipo de redirección es OUTFILE (redirección de salida)...
		{
			fd = open(redir->file, O_WRONLY | O_CREAT | O_TRUNC, 0644); 		// Abre el archivo en modo escritura, truncándolo o creándolo si no existe.
			if (fd == -1)														// Si la apertura falla...
			{
				perror("Error abriendo archivo de salida");						// Imprime un mensaje de error.
				exit(EXIT_FAILURE);												// Sale del programa con un código de error.
			}
			close(fd);															// Cierra el descriptor de archivo, ya que la duplicación fue exitosa.
			cmd->outfd = fd;													// Guarda el descriptor de archivo en el campo 'intfd' de 'cmd' para el seguimiento de la redirección de entrada.
		}
		else if (redir->type == APPEND)											// Si el tipo de redirección es APPEND (redirección de salida en modo append)...
		{
			fd = open(redir->file, O_WRONLY | O_CREAT | O_APPEND, 0644); 		// Abre el archivo en modo append, creándolo si no existe.
			if (fd == -1)														// Si la apertura falla...
			{
				perror("Error abriendo archivo para append");					// Imprime un mensaje de error.
				exit(EXIT_FAILURE);												// Sale del programa con un código de error.
			}
			close(fd);															// Cierra el descriptor de archivo, ya que la duplicación fue exitosa.
			cmd->outfd = fd;													// Guarda el descriptor de archivo en el campo 'outfd' de 'cmd' para el seguimiento de la redirección de salida.
		}
		redir = redir->next;													// Avanza al siguiente elemento en la lista de redirecciones.
	}
}
