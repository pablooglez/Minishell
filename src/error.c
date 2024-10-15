/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pabloglez <pabloglez@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/01 19:55:18 by pablogon          #+#    #+#             */
/*   Updated: 2024/10/15 20:48:09 by pabloglez        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void fatal(int code, char *value)											// Función que imprime mensajes de error personalizados según un código dado.
{
	if (code == MEMORY)														// Si el código es MEMORY imprime un mensaje de error relacionado con la asignación de memoria.
		printf("Error: Memory allocation failed\n");
	if (code == CMD_NOT_FOUND)												// Si el código es MEMORY imprime un mensaje de error relacionado con la asignación de memoria.
		printf("Error: Command not found: %s\n", value);
}

void	ft_error(t_minishell *shell, int code, char *value, int should_exit) // Función para gestionar errores, recibe el shell, un código de error, un valor, y si el programa debe finalizar.
{
	fatal(code, value);														// Llama a la función fatal para imprimir el mensaje de error adecuado.

	if (value) 																// Si se ha pasado un valor no nulo, libera la memoria asignada a 'value'.
		free(value);
	if (should_exit) 														// Si el indicador should_exit está activado, termina el programa con el código de salida que tiene el shell.
		exit(shell->exit_status);											//Termina el programa con el código de salida almacenado en shell.
}
