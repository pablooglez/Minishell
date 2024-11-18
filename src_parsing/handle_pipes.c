/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_pipes.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albelope <albelope@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/15 12:43:54 by albelope          #+#    #+#             */
/*   Updated: 2024/11/18 18:02:25 by albelope         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_pipe_errors(char **tokens, int *i)
{
	 //printf("[DEBUG] check_pipe_errors: Iniciando verificación en índice %d, token: '%s'\n", *i, tokens[*i]);

	// Verificar si el token es una pipe seguida de otra pipe (`||`)
	if (tokens[*i] && ft_strlen(tokens[*i]) >= 2 && tokens[*i][0] == '|' && tokens[*i][1] == '|')
	{
		print_error("Minishell: syntax error near unexpected token `||'\n");
		 //printf("[DEBUG] check_pipe_errors: Error - encontrado '||'\n");
		return (SYNTAX_ERROR);
	}

	// Verificar si la pipe está al principio
	if (*i == 0)
	{
		print_error("Minishell: syntax error near unexpected token `|'\n");
		 //printf("[DEBUG] check_pipe_errors: Error - pipe al principio en índice %d\n", *i);
		return (SYNTAX_ERROR);
	}

	// Verificar si la pipe es el último token
	if (!tokens[*i + 1])
	{
		print_error("Minishell: syntax error near unexpected token `|'\n");
		 //printf("[DEBUG] check_pipe_errors: Error - pipe al final en índice %d\n", *i);
		return (SYNTAX_ERROR);
	}

	// Verificar si hay pipes consecutivas (`| |`)
	if (tokens[*i + 1] && ft_strncmp(tokens[*i + 1], "|", 2) == 0)
	{
		print_error("Minishell: syntax error near unexpected token `|'\n");
		 //printf("[DEBUG] check_pipe_errors: Error - pipes consecutivas en índice %d\n", *i);
		return (SYNTAX_ERROR);
	}

	 //printf("[DEBUG] check_pipe_errors: No se encontraron errores en índice %d\n", *i);
	return (0);
}

int	create_next_command(t_cmd **cmd, t_minishell *shell)
{
	 //printf("[DEBUG] create_next_command: Creando nuevo comando\n");
	(*cmd)->next = create_new_command(shell);
	if (!(*cmd)->next)
	{
		 //printf("[ERROR] create_next_command: Error al crear nuevo comando\n");
		return (-1);
	}
	//printf("[DEBUG] create_next_command: Nuevo comando creado en dirección: %p\n", (void *)(*cmd)->next);
	(*cmd)->next->prev = *cmd;
	*cmd = (*cmd)->next;
	if (*cmd == NULL)
	{
		//printf("[ERROR] create_next_command: current_cmd es NULL después de actualizar\n");
		return (-1);
	}
	//printf("[DEBUG] create_next_command: Comando creado exitosamente\n");
	return (0);
}

int	process_token_pipe(char **tokens, int *i, t_cmd **cmd, t_minishell *shell)
{
	//printf("[DEBUG] process_token_pipe: Procesando token en índice %d: '%s'\n", *i, tokens[*i]);

	if (tokens[*i] && ft_strncmp(tokens[*i], "|", 2) == 0)
	{
		//printf("[DEBUG] process_token_pipe: Token es una pipe en índice %d\n", *i);
		if (check_pipe_errors(tokens, i) == SYNTAX_ERROR)
		{
			shell->exit_status = 2;
			//printf("[ERROR] process_token_pipe: Error de sintaxis detectado en check_pipe_errors\n");
			return (-1);
		}
		//printf("[DEBUG] process_token_pipe: Tipo de comando actualizado a PIPE para dirección: %p\n", (void *)(*cmd));

		if (create_next_command(cmd, shell) == -1)
		{
			//printf("[ERROR] process_token_pipe: Error al crear el siguiente comando\n");
			return (-1);
		}
		(*cmd)->prev->type = PIPE;
		(*i)++;
		//printf("[DEBUG] process_token_pipe: Avanzando al siguiente token después de la pipe, índice %d\n", *i);

		return (0);
	}

	//printf("[DEBUG] process_token_pipe: Token no es una pipe en índice %d: '%s'\n", *i, tokens[*i]);
	return (1);
}

