/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albelope <albelope@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/15 12:53:22 by albelope          #+#    #+#             */
/*   Updated: 2024/10/16 14:24:28 by albelope         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

/* CREAR NUEVA ESTRUCTURA T_CMD
    Cada vez que el usuario ingresa un comando le damos e inicialimos los
    valores, almacenado toda esa informacion en una estrcutura 
    organizada (comandos, argumentos, redirecciones, pipes.., etc.).
    Utilizmos ft_safe_malloc que ya estaba implementada, para asegurar la
    asignacion de memoria. 
    Inicializa todos los campos a NULL porque no sabemos aun que comando es.
    Pipes a -1 pq aun no estan conectados a ningun pipe.
    Pipe[0] = descriptor de lectura.
    Pipe[1] = descriptor de escritura.
    next y prev para conectar y manejar multiples comandos.
    */

t_cmd	*create_new_command(t_minishell *shell)
{
	t_cmd	*new_cmd;

	new_cmd = ft_safe_malloc(shell, sizeof(t_cmd));
	if (!new_cmd)
		return (NULL);
	new_cmd->path = NULL;
	new_cmd->arguments = NULL;
	new_cmd->redir = NULL;
	new_cmd->pipe[0] = -1;
	new_cmd->pipe[1] = -1;
	new_cmd->next = NULL;
	new_cmd->prev = NULL;
	return (new_cmd);
}