/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_input.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albelope <albelope@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/11 19:22:39 by albelope          #+#    #+#             */
/*   Updated: 2024/11/16 20:45:42 by albelope         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


char	*normalize_input(char *input)
{
	char	*norm;
	int		new_len;

	if (!input || is_empty_or_whitespace(input))
		return (input);
	
	//printf("[DEBUG]-->NORMALIZE_INPUT[0.0]==> Input:               [%s]\n", input);
	replace_tabs_with_spaces(input);
	//printf("[DEBUG]-->NORMALIZE_INPUT[0.1]==> Input after replace:  [%s]\n", input);
	norm = ft_calloc(ft_strlen(input) + 1, sizeof(char));
	if (!norm)
		return (input);
	//printf("[DEBUG]-->NORMALIZE_INPUT[0..2]==> Normalizing input norm: [%s]\n", norm);
	new_len = remove_extra_spaces(input, norm);
	//printf("[DEBUG]-->NORMALIZE_INPUT[0.3]==> Normalized:          [%s]\n", norm);
	norm[new_len] = '\0';
	free(input);
	return (norm);
}

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

char	*read_input(void)
{
	char	*input;

	display_prompt();                                                                     // Muestra el prompt personalizado
    input = readline("\033[1;36mMinishell ➜ \033[0m");                                   // Lee la entrada del usuario
	//printf("[DEBUG]-->READ_INPUT[0.1]==> Input = readline() es::               [%s]\n", input);
    if (!input)                                                                           // Detectar EOF (Ctrl+D)
		return (NULL);                                                            // Retornar NULL si se detecta EOF
	if (input[0] != '\0')                                                                 // Verificar si la entrada no está vacía
		add_history(input);                                                               // Agregar al historial si la entrada no es vacía
	if (is_empty_or_whitespace(input))
	{
		free(input);
		return NULL;
	}
	input = normalize_input(input);                                                      // Normalizar la entrada del usuario
	return (input);                                                                       // Retornar la entrada del usuario
}




