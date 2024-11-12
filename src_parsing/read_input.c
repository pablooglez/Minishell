/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_input.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albelope <albelope@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/11 19:22:39 by albelope          #+#    #+#             */
/*   Updated: 2024/11/12 11:25:10 by albelope         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Muestra el prompt con el directorio actual
static void	display_prompt(void)
{       
	char	cwd[1024];                                                                  // Buffer para almacenar el directorio actual

	if (getcwd(cwd, sizeof(cwd)) != NULL)                                               // Obtener el directorio actual
		printf("\033[1;32m%s\033[0m ", cwd);                                            // Imprimir el directorio actual en color verde
	else
	{
		perror("getcwd");                                                                // Mostrar error si falla getcwd
		printf("\033[1;32munknown\033[0m ");                                             // Mostrar "unknown" si getcwd falla
	}
}

// Lee la entrada del usuario y gestiona el historial
char	*read_input(void)
{
	char	*input;

	display_prompt();                                                                     // Muestra el prompt personalizado
    input = readline("\033[1;36mMinishell ➜ \033[0m");                                   // Lee la entrada del usuario
	//printf("[DEBUG]-->READ_INPUT[0.1]==> Input = readline() es::               [%s]\n", input);
    if (!input)                                                                           // Detectar EOF (Ctrl+D)
		return (NULL);                                                                    // Retornar NULL si se detecta EOF
	if (input[0] != '\0')                                                                 // Verificar si la entrada no está vacía
		add_history(input);                                                               // Agregar al historial si la entrada no es vacía
	return (input);                                                                       // Retornar la entrada del usuario
}




