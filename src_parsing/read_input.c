/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_input.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albelope <albelope@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/11 19:22:39 by albelope          #+#    #+#             */
/*   Updated: 2024/10/31 15:44:34 by albelope         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*read_input(void)
{
	char	*input;                                                                             // Variable para almacenar la entrada del usuario

	input = readline(CYAN "Minishell>" WHITE);                                                   // Muestra el prompt y captura la entrada del usuario
	if (!input)                                                                                  // Verifica si la entrada es NULL (EOF o Ctrl+D)
	{
		printf("Exit....\n");                                                                    // Muestra el mensaje "Exit...."
		return (NULL);                                                                           // Retorna NULL para indicar que no hay más entrada
	}
	if (*input != '\0')                                                                          // Verifica si la entrada no está vacía
		add_history(input);                                                                      // Agrega la entrada al historial de comandos
	return (input);                                                                              // Devuelve la cadena de entrada capturada
}



