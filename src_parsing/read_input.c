/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_input.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pabloglez <pabloglez@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/11 19:22:39 by albelope          #+#    #+#             */
/*   Updated: 2024/11/05 13:43:41 by pabloglez        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char *read_input(void)
{
    char	*input;
    char	cwd[1024];                                                  // Buffer para almacenar la ruta actual

    if (getcwd(cwd, sizeof(cwd)) != NULL)                               // Obtener el directorio actual
        printf("\033[1;32m%s\033[0m ", cwd);                            // Imprimir el directorio actual en color verde
    else
        perror("getcwd");                                               // Manejar errores de getcwd si falla
    input = readline("\033[1;36mMinishell ➜ \033[0m");                  // Mostrar prompt con color y leer la entrada del usuario
    if (!input)                                                         // Detectar EOF (Ctrl+D)
        return (NULL);                                                  // Retornar NULL para indicar EOF
    if (input[0] != '\0')                                               // Verificar si la entrada no está vacía
        add_history(input);                                             // Agregar al historial si la entrada no es vacía
    return (input);                                                     // Retornar la entrada del usuario
}




