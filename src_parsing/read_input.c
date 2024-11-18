/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_input.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pabloglez <pabloglez@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/11 19:22:39 by albelope          #+#    #+#             */
/*   Updated: 2024/11/18 21:02:46 by pabloglez        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*static void	display_prompt(void)
{       
	char	cwd[1024];                                                                  // Buffer para almacenar el directorio actual

	if (getcwd(cwd, sizeof(cwd)) != NULL)                                               // Obtener el directorio actual
		printf("\033[1;32m%s\033[0m ", cwd);                                            // Imprimir el directorio actual en color verde
	else
	{
		perror("getcwd");                                                                // Mostrar error si falla getcwd
		printf("\033[1;32munknown\033[0m ");                                             // Mostrar "unknown" si getcwd falla
	}
}*/

char	*read_input(void)
{
	char	*input;

	//display_prompt();                                                                     // Muestra el prompt personalizado
    input = readline("\033[1;36mminishell ➜ \033[0m");                                   // Lee la entrada del usuario
    if (!input)                                                                           // Detectar EOF (Ctrl+D)
		return (NULL);                                                            // Retornar NULL si se detecta EOF
	if (input[0] != '\0')                                                                 // Verificar si la entrada no está vacía
		add_history(input);                                                               // Agregar al historial si la entrada no es vacía
	if (is_empty_or_whitespace(input))
	{
		free(input);
		return NULL;
	}
	return (input);                                                                       // Retornar la entrada del usuario
}




