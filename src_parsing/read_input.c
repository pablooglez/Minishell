/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_input.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pabloglez <pabloglez@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/11 19:22:39 by albelope          #+#    #+#             */
/*   Updated: 2024/11/19 20:59:55 by pabloglez        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*read_input(void)
{
	char	*input;

                                                                  // Llama a la función para mostrar el prompt
	input = readline("\001\033[1;36m\002Minishell ➜ \001\033[0m\002");                                   // Lee la entrada del usuario con readline
	if (!input)                                                                           // Si se recibe EOF (Ctrl+D)
	{
		return (NULL);                                                                    // Retorna NULL para indicar fin de la entrada
	}
	if (input[0] != '\0')                                                                 // Comprueba si la entrada no está vacía
		add_history(input);                                                               // Agrega la entrada al historial si no está vacía
	if (is_empty_or_whitespace(input))                                                    // Verifica si la entrada solo tiene espacios o tabulaciones
	{
		free(input);                                                                     // Libera la memoria de la entrada
		return (NULL);                                                                   // Retorna NULL para ignorar la entrada vacía
	}
	return (input);                                                                       // Retorna la entrada del usuario para procesarla
}