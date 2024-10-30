/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_quotes.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albelope <albelope@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/12 20:32:11 by albelope          #+#    #+#             */
/*   Updated: 2024/10/30 20:09:22 by albelope         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

bool	is_quote(char c)
{
	return (c == '\'' || c == '\"');                                                          // Devuelve true si el carácter es una comilla simple o doble
}

int	handle_escape(char *input, int i, char *temp, int *k)
{
	i++;                                                                                     // Avanza al carácter después del backslash
	if (input[i] == '"' || input[i] == '$' || input[i] == '\\')                              // Permite escapes de `"`, `$`, y `\`
		temp[(*k)++] = input[i++];                                                           // Copia el carácter escapado en `temp` y avanza el índice `i`
	else
	{
		temp[(*k)++] = '\\';                                                                // Copia el backslash literal en `temp`
		temp[(*k)++] = input[i++];                                                          // Copia el siguiente carácter en `temp` y avanza `i`
	}
	return (i);                                                                             // Devuelve el índice actualizado
}

int	handle_single_quotes(char *input, int i, char **tokens, int *j)
{
	int		start;
	char	*token_content;
	char	*token_with_quotes;
	char	*final_token;

	start = i + 1;                                                                          // Marca el inicio del contenido dentro de las comillas simples
	i++;                                                                                    // Avanza más allá de la comilla inicial
	while (input[i] && input[i] != '\'')                                                    // Recorre hasta encontrar la comilla de cierre
		i++;
	if (input[i] != '\'')                                                                   // Verifica si no hay comilla de cierre
		return (-1);                                                                        // Retorna -1 para indicar error
	token_content = ft_substr(input, start, i - start);                                     // Extrae el contenido entre comillas
	if (!token_content)                                                                     // Verifica si hubo un error al crear el token
	{
		free_tokens_parse(tokens);                                                          // Libera tokens en caso de error
		return (-1);                                                                        // Retorna -1 para indicar error
	}
	token_with_quotes = ft_strjoin("'", token_content);                                     // Añade la comilla inicial al contenido extraído
	final_token = ft_strjoin(token_with_quotes, "'");                                       // Añade la comilla final al contenido completo
	free(token_content);                                                                    // Libera memoria temporal de `token_content`
	free(token_with_quotes);                                                                // Libera memoria temporal de `token_with_quotes`
	tokens[*j] = final_token;                                                               // Asigna el token finalizado al array de tokens
	(*j)++;                                                                                 // Incrementa el índice para el próximo token
	return (i + 1);                                                                         // Avanza más allá de la comilla de cierre y devuelve el índice
}

int	handle_double_quotes(char *input, int i, char **tokens, int *j)
{
	char	temp[1024];                                                                     // Buffer temporal para almacenar el contenido procesado
	int		k;

	k = 0;                                                                                  // Inicializa el índice del buffer `temp`
	i++;                                                                                    // Avanza más allá de la comilla inicial
	while (input[i] && input[i] != '"')                                                     // Recorre hasta encontrar la comilla de cierre
	{
		if (input[i] == '\\')                                                               // Si encuentra un backslash
			i = handle_escape(input, i, temp, &k);                                          // Llama a `handle_escape` para procesar el escape
		else
			temp[k++] = input[i++];                                                         // Copia el carácter actual a `temp` y avanza `i`
	}
	if (input[i] != '"')                                                                    // Verifica si no hay comilla de cierre
		return (-1);                                                                        // Retorna -1 para indicar error
	i++;                                                                                    // Avanza más allá de la comilla de cierre
	temp[k] = '\0';                                                                         // Termina el contenido del buffer con un carácter nulo
	tokens[*j] = ft_strdup(temp);                                                           // Crea una copia del contenido de `temp` y la almacena en tokens
	if (!tokens[*j])                                                                       // Verifica si hubo un error al asignar el token
	{
		free_tokens_parse(tokens);                                                          // Libera tokens en caso de error
		return (-1);                                                                        // Retorna -1 para indicar error
	}
	(*j)++;                                                                                 // Incrementa el índice para el próximo token
	return (i);                                                                             // Devuelve el índice actualizado
}

int	handle_quotes(char *input, int i, char **tokens, int *j)
{
	char	quote_char;

	quote_char = input[i];                                                                  // Almacena el carácter de comilla detectado
	if (quote_char == '\'')                                                                 // Si es una comilla simple
		return (handle_single_quotes(input, i, tokens, j));                                 // Llama a `handle_single_quotes` para procesarlo
	else if (quote_char == '"')                                                             // Si es una comilla doble
		return (handle_double_quotes(input, i, tokens, j));                                 // Llama a `handle_double_quotes` para procesarlo
	return (-1);                                                                           // Retorna -1 si no se reconoce el tipo de comilla
}