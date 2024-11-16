/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_pipes.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albelope <albelope@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/15 12:43:54 by albelope          #+#    #+#             */
/*   Updated: 2024/11/16 22:29:50 by albelope         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_pipe_errors(char **tokens, int *i)
{
	if (*i == 0 || (tokens[*i + 1] && ft_strncmp(tokens[*i + 1], "|", 2) == 0))      /* Verificar si el pipe está al principio o seguido de otro pipe */
	{
		print_error("Minishell: syntax error near unexpected token `|'\n");          /* Imprimir error de sintaxis */
		return (SYNTAX_ERROR);                                                      /* Retorna el código de error de sintaxis */
	}
	if (!tokens[*i + 1])                                                            /* Verificar si el pipe es el último token */
	{
		print_error("Minishell: syntax error near unexpected token `|'\n");          /* Imprimir error de sintaxis */
		return (SYNTAX_ERROR);                                                      /* Retorna el código de error de sintaxis */
	}
	return (0);                                                                     /* Retorna 0 si no hay errores */
}

int	create_next_command(t_cmd **cmd, t_minishell *shell)
{
	(*cmd)->next = create_new_command(shell);                                       /* Crear un nuevo comando */
	if (!(*cmd)->next)                                                              /* Verificar si la creación falló */
		return (-1);                                                                /* Retorna -1 si hay un error */
	(*cmd)->next->prev = *cmd;                                                      /* Establece el enlace al comando anterior */
	*cmd = (*cmd)->next;                                                            /* Avanza al siguiente comando */
	return (0);                                                                     /* Retorna 0 indicando éxito */
}

int	process_token_pipe(char **tokens, int *i, t_cmd **cmd, t_minishell *shell)
{
	if (tokens[*i] && ft_strncmp(tokens[*i], "|", 2) == 0)                          /* Verificar si el token actual es un pipe */
	{
		if (check_pipe_errors(tokens, i) == SYNTAX_ERROR)                           /* Verificar errores de sintaxis */
		{
			shell->exit_status = 2;                                                 /* Establecer código de salida a 2 (error de sintaxis) */
			return (-1);                                                            /* Retorna -1 para detener el procesamiento */
		}
		if (!tokens[*i + 1])                                                        /* Verificar si el pipe es el último token , aqui deberia terminar y no seugir ni actualizar shell status*/
		{
			shell->exit_status = 0;                                                 /* Alinear el código de salida con Bash aunque es por si hay que implementar > que seria una putada*/
			return (0);                                                             /* Retorna 0 indicando éxito */
		}
		if (create_next_command(cmd, shell) == -1)                                  /* Crear el siguiente comando */
			return (-1);                                                            /* Retorna -1 si hay un error */

		(*i)++;                                                                     /* Incrementa el índice después de procesar el pipe */
		return (0);                                                                 /* Retorna 0 indicando éxito */
	}
	return (1);                                                                     /* Retorna 1 si el token no es un pipe */
}