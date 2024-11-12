/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_pipes.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albelope <albelope@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/15 12:43:54 by albelope          #+#    #+#             */
/*   Updated: 2024/11/12 16:48:05 by albelope         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int check_pipe_errors(char **tokens, int *i)
{
	if (*i == 0 || !tokens[*i + 1] || ft_strncmp(tokens[*i + 1], "|", 2) == 0)		// Verifica si el pipe está en la posición incorrecta
	{
		printf("Error: Pipe mal posicionado.\n");									// Imprime un mensaje de error si el pipe está mal posicionado
		return (-1);																// Retorna -1 indicando un error
	}
	(*i)++;																			// Incrementa el índice de tokens
	return (0);																		// Retorna 0 indicando que no hubo errores
}

int create_next_command(t_cmd **cmd, t_minishell *shell)
{
	(*cmd)->next = create_new_command(shell);										// Crea un nuevo comando y lo asigna al siguiente nodo
	if (!(*cmd)->next)																// Verifica si la creación del nuevo comando falló
		return (-1);																// Retorna -1 indicando un error
	(*cmd)->next->prev = *cmd;														// Establece el nodo anterior del nuevo comando
	*cmd = (*cmd)->next;															// Avanza el puntero cmd al nuevo comando
	return (0);																		// Retorna 0 indicando éxito
}

int process_token_pipe(char **tokens, int *i, t_cmd **cmd, t_minishell *shell)
{
	if (ft_strncmp(tokens[*i], "|", 2) == 0)										// Verifica si el token actual es un pipe
	{
		
		if (check_pipe_errors(tokens, i) == -1)										// Verifica si hay errores en la posición del pipe
			return (-1);															// Retorna -1 si hay un error
		if (create_next_command(cmd, shell) == -1)									// Crea el siguiente comando
			return (-1);															// Retorna -1 si hay un error
				// Imprime un mensaje de depuración
		return (0);																	// Retorna 0 indicando éxito
	}
	return (1);																		// Retorna 1 si el token no es un pipe
}

