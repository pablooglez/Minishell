/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_input.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pabloglez <pabloglez@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/11 19:22:39 by albelope          #+#    #+#             */
/*   Updated: 2024/10/23 20:12:49 by pabloglez        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*read_input(void)										// Función que se encarga de leer la entrada del usuario.
{
	char	*input;

	input = readline("\033[1;36mminishell ➜\033[0m ");			// Muestra un prompt personalizado (en color) y lee la entrada del usuario con readline.
	if (input && input[0] != '\0')								// Si la entrada no es nula y no está vacía...
		add_history(input);										// ...se añade la entrada a la historia de comandos para poder recuperarla luego con las teclas de flecha.
	
																// Si la entrada es nula, significa que se recibió EOF (Ctrl+ D)
																//Si la entrada es un valor no nulo y está vacía, indica que el usuario presionó Ctrl + C
	if (!input)
		return (NULL);											// Retorna NULL si se recibe EOF
	return(input);												// Retorna la entrada del usuario
}
