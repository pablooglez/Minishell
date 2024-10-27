/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_pipes.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albelope <albelope@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/15 12:43:54 by albelope          #+#    #+#             */
/*   Updated: 2024/10/27 13:33:51 by albelope         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

/*
** Verifica si el pipe esta bien, si no hay comando antes del pipe (i = 0) o después.
** También verifica si hay dos pipes consecutivos sin un comando en medio.
*/
int	check_pipe_errors(char **tokens, int *i)
{
	if (*i == 0)                                                                            // Si el pipe es el primer token (sin comando antes)
	{
		printf("Error: falta un comando antes del pipe.\n");                             // Imprime mensaje de error
		return (-1);                                                                        // Retorna -1 indicando un error
	}
	(*i)++;                                                                                 // Avanza al siguiente token después del pipe
	if (!tokens[*i])                                                                       // Verifica si no hay más tokens después del pipe
	{
		printf("Error: falta un comando después del pipe.\n");                          // Imprime mensaje de error
		return (-1);                                                                        // Retorna -1 indicando un error
	}
	if (ft_strncmp(tokens[*i], "|", 2) == 0)                                                // Si el siguiente token también es un pipe
	{
		printf("Error: falta un comando entre los pipes.\n");                           // Imprime mensaje de error
		return (-1);                                                                        // Retorna -1 indicando un error
	}
	return (0);                                                                             // Retorna 0 si no hay errores con el pipe
}

/*
** Crea un nuevo comando en la lista doblemente enlazada cuando se encuentra un pipe.
** Actualiza el puntero `cmd` para apuntar al nuevo comando.
** Devuelve -1 si ocurre un error en la creación del comando, 0 si todo está bien.
*/
int	create_next_command(t_cmd **cmd, t_minishell *shell)
{
	(*cmd)->next = create_new_command(shell);                                               // Crea un nuevo nodo `t_cmd` para el siguiente comando
	if (!(*cmd)->next)                                                                      // Verifica si hubo un error al crear el nuevo comando
		return (-1);                                                                        // Retorna -1 indicando un error
	(*cmd)->next->prev = *cmd;                                                              // Conecta el nuevo comando al comando actual (enlazado doble)
	*cmd = (*cmd)->next;                                                                    // Actualiza el puntero para que apunte al nuevo comando
	return (0);                                                                             // Retorna 0 si se creó correctamente el nuevo comando
}

/*
** Procesa un token de pipe ('|') en la cadena de tokens.
** Si encuentra un pipe, verifica si está correctamente colocado y crea un nuevo comando.
** Devuelve:
** -1 en caso de error.
**  0 si encuentra un pipe y todo está bien.
**  1 si el token actual no es un pipe, para que se continúe con otros tokens.
*/
int	process_token_pipe(char **tokens, int *i, t_cmd **cmd, t_minishell *shell)
{
	if (ft_strncmp(tokens[*i], "|", 2) == 0)                                                // Verifica si el token actual es un pipe ('|')
	{
		if (check_pipe_errors(tokens, i) == -1)                                             // Llama a `check_pipe_errors` para validar la posición del pipe
			return (-1);                                                                    // Retorna -1 si se detectó un error
		if (create_next_command(cmd, shell) == -1)                                          // Crea un nuevo comando para lo que sigue después del pipe
			return (-1);                                                                    // Retorna -1 si hubo un error al crear el nuevo comando
		return (0);                                                                         // Retorna 0 si el pipe fue procesado correctamente
	}
	return (1);                                                                             // Retorna 1 si el token no es un pipe
}

// QUEDA MANEJAR QUE SE LIBERE LA MEMORIA CORRECTAMENTE Y VER LA MODULARIDAD DEL CODIGO AUN