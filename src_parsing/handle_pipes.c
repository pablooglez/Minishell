/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_pipes.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albelope <albelope@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/15 12:43:54 by albelope          #+#    #+#             */
/*   Updated: 2024/10/29 22:19:44 by albelope         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

/* 
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

int	create_next_command(t_cmd **cmd, t_minishell *shell)
{
	(*cmd)->next = create_new_command(shell);                                               // Crea un nuevo nodo `t_cmd` para el siguiente comando
	if (!(*cmd)->next)                                                                      // Verifica si hubo un error al crear el nuevo comando
		return (-1);                                                                        // Retorna -1 indicando un error
	(*cmd)->next->prev = *cmd;                                                              // Conecta el nuevo comando al comando actual (enlazado doble)
	*cmd = (*cmd)->next;                                                                    // Actualiza el puntero para que apunte al nuevo comando
	return (0);                                                                             // Retorna 0 si se creó correctamente el nuevo comando
}

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
*/



int	check_pipe_errors(char **tokens, int *i)
{
	if (*i == 0 || !tokens[*i + 1] || ft_strncmp(tokens[*i + 1], "|", 2) == 0)
	{
		printf("Error: Pipe mal posicionado.\n");
		return (-1);
	}
	(*i)++;
	return (0);
}

int	create_next_command(t_cmd **cmd, t_minishell *shell)
{
	(*cmd)->next = create_new_command(shell);
	if (!(*cmd)->next)
		return (-1);
	(*cmd)->next->prev = *cmd;
	*cmd = (*cmd)->next;
	return (0);
}

int	process_token_pipe(char **tokens, int *i, t_cmd **cmd, t_minishell *shell)
{
	if (ft_strncmp(tokens[*i], "|", 2) == 0)
	{
		if (check_pipe_errors(tokens, i) == -1)
			return (-1);
		if (create_next_command(cmd, shell) == -1)
			return (-1);
		return (0);
	}
	return (1);
}
