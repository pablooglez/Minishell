/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_char.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albelope <albelope@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/13 17:49:52 by albelope          #+#    #+#             */
/*   Updated: 2024/10/25 13:22:15 by albelope         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

/*
** Verificar si el carácter es un carácter especial (pipes, redirecciones, etc.)
** en la sintaxis de shell (Minishell).
*/
int	is_special_char(char c)
{
	return (c == '|' || c == '>' || c == '<');                                              // Devuelve true si el carácter es un pipe o redirección
}

/*
** Verifica si la cadena de entrada contiene caracteres inválidos para el shell.
** Retorna true si encuentra ';' o '\\', que no son permitidos en Minishell.
*/
bool	contains_invalid_characters(char *input)
{
	int	i;

	i = 0;                                                                                  // Inicializa el índice para recorrer la cadena
	while (input[i])                                                                       // Recorre la cadena hasta el final
	{
		if (input[i] == ';' || input[i] == '\\')                                            // Verifica si el carácter es ';' o '\\'
		{
			printf("Erroritos: uso de caracteres especiales no permitidos (%c).\n",        // Imprime un mensaje de error si se detecta un carácter inválido
				   input[i]);
			return (true);                                                                 // Retorna true indicando que hay caracteres no permitidos
		}
		i++;                                                                               // Avanza al siguiente carácter
	}
	return (false);                                                                        // Retorna false si no hay caracteres no permitidos
}

/*
** Manejar la tokenización de caracteres especiales -- incluyendo redirecciones
** simples y dobles.
** Recibe la cadena que el usuario introduce (input), el índice actual de la cadena `i`,
** el array `tokens` donde se almacenarán los tokens generados y `*j` el índice actual
** en el array de tokens (la posición donde se almacenará el siguiente token).
** 
** Usa un puntero temporal `temp` para almacenar el token que representa el carácter
** especial. Verifica si es una redirección simple o doble y extrae 1 o 2 caracteres
** con `ft_substr`. Almacena el contenido en `tokens[*j]` y avanza `*j++` para asegurar
** que el siguiente token se almacene en la siguiente posición vacía del array.
** 
** Devuelve `i + 1` o `i + 2` para saltar los caracteres que ya hemos procesado.
*/
int	handle_special_char(char *input, int i, char **tokens, int *j)
{
	char	*temp;

	temp = NULL;                                                                           // Inicializa `temp` a NULL
	if ((input[i] == '>' && input[i + 1] == '>') || (input[i] == '<' && input[i + 1] == '<'))
	{                                                                                      // Verifica si es una redirección doble (`>>` o `<<`)
		temp = ft_substr(input, i, 2);                                                     // Extrae dos caracteres desde `input[i]`
		if (!temp)                                                                        // Verifica si hubo un error al asignar `temp`
		{
			free_tokens_parse(tokens);                                                    // Libera el array de tokens en caso de error
			return (-1);                                                                  // Retorna -1 indicando un error
		}
		tokens[*j] = temp;                                                                // Asigna el token extraído al array de tokens
		(*j)++;                                                                           // Incrementa el índice para el próximo token
		return (i + 2);                                                                   // Avanza dos posiciones en la cadena `input`
	}
	temp = ft_substr(input, i, 1);                                                         // Extrae un solo carácter si es una redirección simple
	if (!temp)                                                                            // Verifica si hubo un error al asignar `temp`
	{
		free_tokens_parse(tokens);                                                        // Libera el array de tokens en caso de error
		return (-1);                                                                      // Retorna -1 indicando un error
	}
	tokens[*j] = temp;                                                                    // Asigna el token extraído al array de tokens
	(*j)++;                                                                               // Incrementa el índice para el próximo token
	return (i + 1);                                                                       // Avanza una posición en la cadena `input`
}