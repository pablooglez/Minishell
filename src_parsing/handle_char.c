/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_char.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pabloglez <pabloglez@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/13 17:49:52 by albelope          #+#    #+#             */
/*   Updated: 2024/11/04 19:43:11 by pabloglez        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_token get_special_token_type(char c)
{
	if (c == '|')
		return (PIPE);																			// Retorna PIPE si el carácter es '|'
	if (c == '>')
		return (REDIR);																			// Retorna REDIR si el carácter es '>'
	if (c == '<')
		return (REDIR);																			// Retorna REDIR si el carácter es '<'
	return (UNKNOWN);																			// Retorna UNKNOWN si no coincide con ningún carácter especial
}

bool contains_invalid_characters(char *input)
{
	int i;

	i = 0;
	while (input[i])																			// Recorre cada carácter en la cadena input
	{
		if (input[i] == ';' || (input[i] == '\\' && input[i + 1] != '$'))						// Verifica si el carácter es ';' o un '\' que no escapa '$'
		{
			printf("Error: uso de caracteres especiales no permitidos (%c).\n", input[i]);		// Imprime un mensaje de error
			return (true);																		// Retorna true si se encuentra un carácter inválido
		}
		i++;
	}
	return (false);																				// Retorna false si no hay caracteres inválidos
}

int handle_special_char(char *input, int i, char **tokens, int *j)
{
	char	*temp;
	t_token	type;

	type = get_special_token_type(input[i]);													// Determina el tipo de token especial para el carácter actual
	temp = NULL;

	if ((input[i] == '>' && input[i + 1] == '>') || (input[i] == '<' && input[i + 1] == '<'))	// Verifica si el carácter es una redirección doble
	{
		temp = ft_substr(input, i, 2);															// Extrae dos caracteres como un solo token
		if (!temp)
			return (-1);																		// Retorna -1 si falla la asignación de memoria
		tokens[*j] = temp;																		// Almacena el token en tokens
		(*j)++;																					// Incrementa el índice de tokens
		return (i + 2);																			// Avanza dos posiciones en la cadena
	}
	else if (type != UNKNOWN)																	// Si es un token especial de un solo carácter
	{
		temp = ft_substr(input, i, 1);															// Extrae un solo carácter como token
		if (!temp)
			return (-1);																		// Retorna -1 si falla la asignación de memoria
		tokens[*j] = temp;																		// Almacena el token en tokens
		(*j)++;																					// Incrementa el índice de tokens
		return (i + 1);																			// Avanza una posición en la cadena
	}
	return (-1);																				// Retorna -1 si no es un carácter especial conocido
}

