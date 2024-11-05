/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pabloglez <pabloglez@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/11 20:39:51 by albelope          #+#    #+#             */
/*   Updated: 2024/11/04 21:13:44 by pabloglez        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_token	classify_special_token(char c)																				// Clasifica un carácter especial y retorna su tipo como t_token
{
	if (c == '|')																									// Si el carácter es un pipe
		return (PIPE);																								// Retorna el token PIPE
	if (c == '>')																									// Si el carácter es un símbolo de redirección de salida
		return (REDIR);																								// Retorna el token REDIR
	if (c == '<')																									// Si el carácter es un símbolo de redirección de entrada
		return (REDIR);																								// Retorna el token REDIR
	return (UNKNOWN);																								// Retorna UNKNOWN si el carácter no es especial
}


void	free_tokens_parse(char **tokens)																			// Libera la memoria asignada para un array de tokens
{
	int	i;																											// Declara una variable entera para recorrer los tokens

	i = 0;																											// Inicializa i en 0
	if (!tokens)																									// Si tokens es NULL, no hay nada que liberar
		return ;																									// Sale de la función sin hacer nada
	while (tokens[i])																								// Recorre los tokens hasta encontrar un NULL
		free(tokens[i++]);																							// Libera el token actual y luego incrementa i
	free(tokens);																									// Finalmente, libera el array de punteros en sí mismo
}

int	handle_token(char *input, int i, char **tokens, int *j)															// Procesa un token que no es una comilla ni carácter especial
{
	int	start;																										// Declara una variable para almacenar el índice inicial del token

	start = i;																										// Asigna el índice actual como el inicio del token
	while (input[i] && input[i] != ' ' && !is_quote(input[i]) && get_special_token_type(input[i]) == UNKNOWN)
		i++;																										// Avanza el índice mientras el carácter no sea espacio, comilla o carácter especial
	tokens[*j] = ft_substr(input, start, i - start);																// Extrae el substring y lo asigna a tokens[j]
	if (!tokens[*j])																								// Si la asignación falla
	{
		free_tokens_parse(tokens);																					// Libera tokens y retorna -1
		return (-1);
	}
	(*j)++;																											// Incrementa j para el próximo token
	return (i);																										// Retorna el índice actualizado
}

int	process_token(char *input, int *i, char **tokens, int *j)														// Procesa un token en la posición actual del input
{
	if (is_quote(input[*i]))																						// Si el carácter es una comilla
	{
		*i = handle_quotes(input, *i, tokens, j);																	// Maneja el contenido entre comillas
		if (*i == -1)																								// Si hay un error en el manejo de comillas
			return (-1);																							// Retorna -1 para indicar error
	}
	else if (get_special_token_type(input[*i]) != UNKNOWN)															// Si el carácter es especial
	{
		*i = handle_special_char(input, *i, tokens, j);																// Maneja el carácter especial
		if (*i == -1)																								// Si hay un error
			return (-1);																							// Retorna -1 para indicar error
	}
	else
	{
		*i = handle_token(input, *i, tokens, j);																	// Procesa un token regular
		if (*i == -1)																								// Si hay un error
			return (-1);																							// Retorna -1 para indicar error
	}
	return (0);																										// Retorna 0 para indicar éxito
}

char	**tokenize_input(char *input)																				// Divide la línea de entrada en tokens y los almacena en un array
{
	char	**tokens;																								// Declara un array de strings para almacenar los tokens
	int		i;																										// Declara un índice para recorrer el input
	int		j;																										// Declara un índice para almacenar tokens en el array

	i = 0;																											// Inicializa i en 0
	j = 0;																											// Inicializa j en 0
	tokens = ft_calloc(100, sizeof(char *));																		// Reserva memoria para hasta 100 tokens
	if (!tokens)																									// Si falla la asignación de memoria
		return (NULL);																								// Retorna NULL para indicar error
	while (input[i])																								// Recorre cada carácter del input
	{
		while (input[i] == ' ')																						// Ignora los espacios iniciales
			i++;
		if (process_token(input, &i, tokens, &j) == -1)																// Procesa el token y verifica errores
		{
			free_tokens_parse(tokens);																				// Libera tokens y retorna NULL si hay error
			return (NULL);
		}
		//while (input[i] == ' ')																					// Este código está comentado, ignora los espacios nuevamente (quizás redundante)
		//i++;
	}
	tokens[j] = NULL;																								// Añade un NULL al final de tokens para indicar el final del array
	return (tokens);																								// Retorna el array de tokens
}

