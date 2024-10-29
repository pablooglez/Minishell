/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albelope <albelope@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/11 20:39:51 by albelope          #+#    #+#             */
/*   Updated: 2024/10/29 22:20:48 by albelope         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
/*  
void	free_tokens_parse(char **tokens)
{
	int i;                                                                                     // Índice para recorrer los tokens
	
	i = 0;                                                                                     // Inicializa el índice a 0
	if (!tokens)                                                                              // Si no hay tokens, no hay nada que liberar, retorna directamente
		return;
	while (tokens[i])                                                                         // Recorre cada token hasta encontrar un valor NULL (final de la lista)
	{
		free(tokens[i]);                                                                      // Libera cada cadena de caracteres individualmente
		i++;                                                                                  // Avanza al siguiente token
	}
	free(tokens);                                                                             // Finalmente, libera el array completo de punteros a cadenas
}


int	handle_token(char *input, int i, char **tokens, int *j)
{
	int start;                                                                               // Variable para marcar el inicio del token

	start = i;                                                                               // Guarda la posición inicial del token
	while (input[i] && input[i] != ' ' && !is_quote(input[i]) && !is_special_char(input[i])) // Avanza mientras no haya espacios, comillas o caracteres especiales
		i++;
	tokens[*j] = ft_substr(input, start, i - start);  
	//printf("Token regular procesado: '%s'\n", tokens[*j]);                                       // Extrae la subcadena y la almacena en tokens[*j]
	if (!tokens[*j])                                                                       
	{
		free_tokens_parse(tokens);                                                           // Si falla, libera los tokens existentes y retorna error
		return (-1);
	}
	(*j)++;                                                                                  // Incrementa 'j' para la siguiente posición de token
	return (i);                                                                              // Retorna la nueva posición 'i' después de procesar el token
}


int process_token(char *input, int *i, char **tokens, int *j)
{
	if (is_quote(input[*i]))                                                                 // Detecta si el carácter es una comilla (simple o doble)
	{
		//printf("Encontrada comilla en posición %d: '%c'\n", *i, input[*i]);
		*i = handle_quotes(input, *i, tokens, j);                                            // Maneja las comillas y obtiene el nuevo índice
		if (*i == -1)                                                                        // Verifica si hubo un error (por ejemplo, comilla no cerrada)
			return (-1);                                                                    
	}
	else if (is_special_char(input[*i]))                                                     // Detecta si el carácter es un carácter especial (|, <, >, etc.)
	{
		//printf("Encontrado carácter especial en posición %d: '%c'\n", *i, input[*i]);
		*i = handle_special_char(input, *i, tokens, j);                                      // Maneja los caracteres especiales
		if (*i == -1)                                                                        // Si hay un error, retorna -1
			return (-1);
	}
	else                                                                                     // Si no es comilla ni especial, es un token regular
	{
		//printf("Token regular encontrado en posición %d: '%c'\n", *i, input[*i]);
		*i = handle_token(input, *i, tokens, j);                                             // Procesa el token regular
		if (*i == -1)                                                                        // Verifica si hubo un error
			return (-1);
	}
	return 0;                                                                                // Retorna 0 si todo fue procesado correctamente
}

char	**tokenize_input(char *input)
{
	char	**tokens;                                                                       // Array de punteros para almacenar los tokens
	int		i;                                                                              // Índice para recorrer la cadena de entrada
	int		j;                                                                              // Índice para almacenar tokens en el array

	i = 0;                                                                                   // Inicializa el índice de entrada a 0
	j = 0;                                                                                   // Inicializa el índice de tokens a 0
	tokens = ft_calloc(100, sizeof(char *));    
	//printf("Input recibido para tokenización: '%s'\n", input);                                             // Reserva memoria para un máximo de 100 tokens
	if (!tokens)                                                                            // Verifica si la memoria fue asignada correctamente
		return NULL;	
	while (input[i])                                                                        // Recorre la cadena de entrada hasta el final
	{
		while (input[i] == ' ')                                                              // Salta espacios en blanco
			i++;
		if (process_token(input, &i, tokens, &j) == -1)                                     // Procesa cada token y verifica si hubo algún error
		{
			free_tokens_parse(tokens);                                                      // Libera los tokens si hubo error
			return NULL;
		}
		while (input[i] == ' ')                                                              // Salta espacios después de procesar el token
			i++;
	}
	tokens[j] = NULL;
	//printf("Tokens generados:\n");
    //for (int k = 0; k < j; k++)
	//{
    //    printf("Token [%d]: '%s'\n", k, tokens[k]);
    //}                                                                       // Marca el final del array de tokens con NULL
	return (tokens);                                                                        // Devuelve el array de tokens generado
}
 */



void	free_tokens_parse(char **tokens)
{
	int	i;

	i = 0;
	if (!tokens)
		return ;
	while (tokens[i])
		free(tokens[i++]);
	free(tokens);
}

int	handle_token(char *input, int i, char **tokens, int *j)
{
	int	start;

	start = i;
	while (input[i] && input[i] != ' ' && !is_quote(input[i]) && get_special_token_type(input[i]) == UNKNOWN)
		i++;
	tokens[*j] = ft_substr(input, start, i - start);
	if (!tokens[*j])
	{
		free_tokens_parse(tokens);
		return (-1);
	}
	(*j)++;
	return (i);
}

int	process_token(char *input, int *i, char **tokens, int *j)
{
	if (is_quote(input[*i]))
	{
		*i = handle_quotes(input, *i, tokens, j);
		if (*i == -1)
			return (-1);
	}
	else if (get_special_token_type(input[*i]) != UNKNOWN)
	{
		*i = handle_special_char(input, *i, tokens, j);
		if (*i == -1)
			return (-1);
	}
	else
	{
		*i = handle_token(input, *i, tokens, j);
		if (*i == -1)
			return (-1);
	}
	return (0);
}

char	**tokenize_input(char *input)
{
	char	**tokens;
	int		i;
	int		j;

	i = 0;
	j = 0;
	tokens = ft_calloc(100, sizeof(char *));
	if (!tokens)
		return (NULL);
	while (input[i])
	{
		while (input[i] == ' ')
			i++;
		if (process_token(input, &i, tokens, &j) == -1)
		{
			free_tokens_parse(tokens);
			return (NULL);
		}
		while (input[i] == ' ')
			i++;
	}
	tokens[j] = NULL;
	return (tokens);
}
