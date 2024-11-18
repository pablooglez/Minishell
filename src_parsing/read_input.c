/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_input.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pabloglez <pabloglez@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/11 19:22:39 by albelope          #+#    #+#             */
/*   Updated: 2024/11/18 21:11:52 by pabloglez        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


static void	display_prompt(void)
{
	char	cwd[1024];                                                                  // Buffer para almacenar el directorio actual

	if (getcwd(cwd, sizeof(cwd)) != NULL)                                               // Obtiene el directorio actual
		printf("\033[1;32m%s\033[0m ", cwd);                                            // Imprime el directorio actual en verde
	else
	{
		perror("getcwd");                                                                // Muestra el error si getcwd falla
		printf("\033[1;32munknown\033[0m ");                                             // Muestra "unknown" si no puede obtener el directorio
	}
}

char	*read_input(void)
{
	char	*input;

	display_prompt();                                                                     // Llama a la función para mostrar el prompt
	input = readline("\033[1;36mMinishell ➜ \033[0m");                                   // Lee la entrada del usuario con readline
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





