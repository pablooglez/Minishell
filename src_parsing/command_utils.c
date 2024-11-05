/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pabloglez <pabloglez@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/15 12:53:22 by albelope          #+#    #+#             */
/*   Updated: 2024/11/05 19:28:38 by pabloglez        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_cmd *create_new_command(t_minishell *shell)
{
	t_cmd *new_cmd;

	new_cmd = safe_malloc(shell, sizeof(t_cmd));	// Reserva memoria para un nuevo comando usando safe_malloc
	if (!new_cmd)
		return (NULL);								// Devuelve NULL si no se pudo asignar memoria

	new_cmd->path = NULL;							// Inicializa el campo path en NULL (ruta del comando)
	new_cmd->arguments = NULL;						// Inicializa el campo arguments en NULL (argumentos del comando)
	new_cmd->redir = NULL;							// Inicializa el campo redir en NULL (lista de redirecciones)
	new_cmd->pipe[0] = -1;							// Inicializa el primer descriptor de pipe en -1
	new_cmd->pipe[1] = -1;							// Inicializa el segundo descriptor de pipe en -1
	new_cmd->next = NULL;							// Inicializa el puntero al siguiente comando en NULL
	new_cmd->prev = NULL;							// Inicializa el puntero al comando anterior en NULL
	new_cmd->intfd = 0;
	new_cmd->outfd = 1;
	return (new_cmd);								// Devuelve el nuevo comando inicializado
}
