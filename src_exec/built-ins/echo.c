/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pabloglez <pabloglez@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/14 19:59:34 by pabloglez         #+#    #+#             */
/*   Updated: 2024/10/15 19:18:45 by pabloglez        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_echo(t_minishell *shell, char **arg)			// Función para manejar el comando echo
{
	(void)shell;										//Evitar advertencia de parámetro no utilizado
	bool newline_flag = true;							// Inicializa la opción de salto de línea a verdadero
	int i;												// Iniciar con el primer argumento, ya que el índice 0 es el propio comando "echo"
	i = 1;
														// Comprobar si el primer argumento es la opción "-n"
	if (arg[1] && ft_strncmp(arg[1], "-n", 3) == 0)		// Compara el primer argumento con "-n"
	{
		newline_flag = false;							// Cambia a false para no añadir un salto de línea
		i = 2;											// Ignora el argumento "-n" y comienza a imprimir desde el siguiente
	}
														// Imprimir los argumentos desde el índice i en adelante
	while (arg[i])										// Mientras haya más argumentos
	{
		printf("%s", arg[i]);							// Imprime el argumento actual
		i++;											// Avanza al siguiente índice
		if (arg[i])										// Si hay más argumentos después del actual
			printf(" ");								// Imprime un espacio entre los argumentos
	}
														// Añadir un salto de línea al final si la opción no se configuró
	if (newline_flag)
		printf("\n");									// Imprime un salto de línea
}
