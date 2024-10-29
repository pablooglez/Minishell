/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_char.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albelope <albelope@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/13 17:49:52 by albelope          #+#    #+#             */
/*   Updated: 2024/10/29 22:19:56 by albelope         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"


/*int	is_special_char(char c)
{
	return (c == '|' || c == '>' || c == '<');                                              // Devuelve true si el carácter es un pipe o redirección
}

bool	contains_invalid_characters(char *input)
{
	int	i;

	i = 0;                                                                                  // Inicializa el índice para recorrer la cadena
	while (input[i])                                                                       // Recorre la cadena hasta el final
	{
		if (input[i] == ';')                                            // Verifica si el carácter es ';' o '\\'
		{
			printf("Error: uso de caracteres especiales no permitidos (%c).\n",        // Imprime un mensaje de error si se detecta un carácter inválido
				   input[i]);
			return (true);                                                                 // Retorna true indicando que hay caracteres no permitidos
		}
		else if (input[i] == '\\' && input[i + 1] != '$')
		{
			printf("Error: uso de caracteres especiales no permitidos (%c).\n", input[i]);
			return (true);
		}
		i++;                                                                               // Avanza al siguiente carácter
	}
	return (false);                                                                        // Retorna false si no hay caracteres no permitidos
}

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
}*/

#include "../include/minishell.h"

t_token	get_special_token_type(char c)
{
	if (c == '|')
		return (PIPE);
	if (c == '>')
		return (REDIR);
	if (c == '<')
		return (REDIR);
	return (UNKNOWN);
}

bool	contains_invalid_characters(char *input)
{
	int	i;

	i = 0;
	while (input[i])
	{
		if (input[i] == ';' || (input[i] == '\\' && input[i + 1] != '$'))
		{
			printf("Error: uso de caracteres especiales no permitidos (%c).\n", input[i]);
			return (true);
		}
		i++;
	}
	return (false);
}

int	handle_special_char(char *input, int i, char **tokens, int *j)
{
	char	*temp;

	temp = NULL;
	if ((input[i] == '>' && input[i + 1] == '>') || (input[i] == '<' && input[i + 1] == '<'))
	{
		temp = ft_substr(input, i, 2);
		if (!temp)
			return (-1);
		tokens[*j] = temp;
		(*j)++;
		return (i + 2);
	}
	temp = ft_substr(input, i, 1);
	if (!temp)
		return (-1);
	tokens[*j] = temp;
	(*j)++;
	return (i + 1);
}
