/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pabloglez <pabloglez@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/15 12:53:22 by albelope          #+#    #+#             */
/*   Updated: 2024/11/12 21:03:16 by pabloglez        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_cmd	*create_new_command(t_minishell *shell)											
{
	t_cmd	*new_cmd;

	new_cmd = safe_malloc(shell, sizeof(t_cmd));									// Asigna memoria para un nuevo comando
	if (!new_cmd)																	// Verifica si la asignación falló
		return (NULL);
	new_cmd->path = NULL;															// Inicializa el path del comando
	new_cmd->arguments = NULL;														// Inicializa los argumentos del comando
	new_cmd->redir = NULL;															// Inicializa las redirecciones del comando
	new_cmd->pipe[0] = -1;															// Inicializa el pipe de entrada
	new_cmd->pipe[1] = -1;															// Inicializa el pipe de salida
	new_cmd->next = NULL;															// Inicializa el puntero al siguiente comando
	new_cmd->prev = NULL;															// Inicializa el puntero al comando anterior
	new_cmd->intfd = 0;																// Inicializa el descriptor de archivo de entrada														
	new_cmd->outfd = 1;																// Inicializa el descriptor de archivo de salida
	return (new_cmd);																// Retorna el nuevo comando
}

/*SE PODRIA INICIALIZAR MEMSET PARA ALGUNOS COMANDOS DE CREATE_NEW_COMMAND*/

int	initialize_arguments(char **tokens, int *i, t_cmd *cmd)
{
	if (!cmd->path)																	// Verifica si el path del comando no ha sido asignado
	{
		/*if (tokens[0][0] == '$')													// Verifica si el primer token es una variable
			cmd->path = ft_strdup("echo");*/											// Asigna el comando "echo" al path
		//else
			cmd->path = ft_strdup(tokens[*i]);										// Duplica el token actual al path
		if (!cmd->path)																// Verifica si la asignación falló
			return (-1);															// Retorna -1 en caso de error
		(*i)++;																		// Incrementa el índice
	}
	cmd->arguments = ft_calloc(100, sizeof(char *));								// Asigna memoria para los argumentos del comando
	if (!cmd->arguments)															// Verifica si la asignación falló
		return (-1);																// Retorna -1 en caso de error
	return (0);
}

int	add_argument(char *token, int arg_index, t_cmd *cmd)
{
	cmd->arguments[arg_index] = ft_strdup(token);									// Duplica el token al array de argumentos
	if (!cmd->arguments[arg_index])													// Verifica si la asignación falló		
		return (-1);																// Retorna -1 en caso de error
	return (0);
}

