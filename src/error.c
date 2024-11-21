/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pabloglez <pabloglez@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/01 19:55:18 by pablogon          #+#    #+#             */
/*   Updated: 2024/11/21 19:19:53 by pabloglez        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	built_ins(int code, char *value)
{
	if (code == MSG && value)
		write(2, value, ft_strlen(value));
	if (code == CD_NOT_FOUND && value)
	{
		write(2, "cd: ", 4);
		write(2, value, ft_strlen(value));
		write(2, ": No such file or directory\n", 29);
	}
}

static void	fatal(int code, char *value)
{
	if (code == MEMORY_ERROR)                                                                            // Manejar error de memoria
	{
		write(2, "Minishell: error: Memory allocation failed\n", 43);                                    // Mensaje de error para fallo de memoria
		exit(EXIT_FAILURE);                                                                              // Termina el programa con EXIT_FAILURE (1)
	}
	else if (code == CMD_NOT_FOUND)                                                                      // Manejar error de comando no encontrado
	{
		write(2, "Minishell: command not found: ", 30);                                                  // Mensaje de error para comando no encontrado
		if (value)                                                                                       // Verificar si value no es NULL
			write(2, value, ft_strlen(value));                                                           // Imprimir el nombre del comando
		write(2, "\n", 1);                                                                               // Imprimir salto de línea
		exit(127);                                                                                       // Termina con código 127 (comando no encontrado)
	}
	else if (code == SYNTAX_ERROR)                                                                       // Manejar error de sintaxis
	{
		write(2, "Minishell: syntax error near unexpected token `", 47);                                 // Mensaje de error para sintaxis
		if (value)                                                                                       // Verificar si value no es NULL
			write(2, value, ft_strlen(value));                                                           // Imprimir el token inesperado
		write(2, "'\n", 2);                                                                              // Imprimir comilla y salto de línea
		exit(2);                                                                                         // Termina con código 2 (error de sintaxis)
	}
	else if (code == PERMISSION_DENIED)                                                                  // Manejar error de permiso denegado
	{
		write(2, "Minishell: permission denied: ", 31);                                                  // Mensaje de error para permiso denegado
		if (value)                                                                                       // Verificar si value no es NULL
			write(2, value, ft_strlen(value));                                                           // Imprimir el nombre del archivo o directorio
		write(2, "\n", 1);                                                                               // Imprimir salto de línea
		exit(126);                                                                                       // Termina con código 126 (permiso denegado)
	}
}

int	ft_error(t_minishell *shell, int code, char *value, int should_exit)							// Función para gestionar errores, recibe el shell, un código de error, un valor, y si el programa debe finalizar.
{
	fatal(code, value);																					// Llama a la función fatal para imprimir el mensaje de error adecuado.
	built_ins(code, value);

	if (code != MSG && value)																						// Si se ha pasado un valor no nulo, libera la memoria asignada a 'value'.
		free(value);
	
	if (should_exit) 																					// Si el indicador should_exit está activado, termina el programa con el código de salida que tiene el shell.
		exit(shell->exit_status);																		//Termina el programa con el código de salida almacenado en shell.
	return (1);
}
