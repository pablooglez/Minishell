/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albelope <albelope@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/11 20:39:51 by albelope          #+#    #+#             */
/*   Updated: 2024/10/25 13:12:58 by albelope         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

/*
   Libera el array de tokens que fue creado durante la tokenización.
   - tokens: es un array de cadenas (strings) que representa los tokens generados
     por el shell a partir de la línea de entrada del usuario.

   La función recorre el array de tokens y libera cada cadena individualmente,
   luego libera el array completo para evitar fugas de memoria.
*/
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

/*
   Esta función procesa un token regular (no es comilla ni carácter especial).
   - input: cadena de entrada del usuario.
   - i: índice actual en la cadena de entrada.
   - tokens: array donde se almacenarán los tokens generados a partir de la entrada.
   - j: índice actual en el array de tokens, que indica dónde almacenar el próximo token.

   Comienza a partir de la posición actual de 'i' y avanza hasta encontrar un espacio,
   una comilla o un carácter especial. Extrae la subcadena desde la posición de inicio
   hasta la posición final y la almacena en tokens[*j]. Incrementa 'j' para indicar que
   el siguiente token se almacenará en la siguiente posición del array.
*/
int	handle_token(char *input, int i, char **tokens, int *j)
{
	int start;                                                                               // Variable para marcar el inicio del token

	start = i;                                                                               // Guarda la posición inicial del token
	while (input[i] && input[i] != ' ' && !is_quote(input[i]) && !is_special_char(input[i])) // Avanza mientras no haya espacios, comillas o caracteres especiales
		i++;
	tokens[*j] = ft_substr(input, start, i - start);                                         // Extrae la subcadena y la almacena en tokens[*j]
	if (!tokens[*j])                                                                       
	{
		free_tokens_parse(tokens);                                                           // Si falla, libera los tokens existentes y retorna error
		return (-1);
	}
	(*j)++;                                                                                  // Incrementa 'j' para la siguiente posición de token
	return (i);                                                                              // Retorna la nueva posición 'i' después de procesar el token
}

/*
   Esta función determina qué tipo de token se está procesando (comilla, especial o normal)
   y llama a la función correspondiente para manejarlo.
   - input: cadena de entrada del usuario.
   - i: puntero al índice actual en la cadena de entrada.
   - tokens: array donde se almacenarán los tokens generados.
   - j: índice actual en el array de tokens.

   La función detecta si el carácter en input[*i] es una comilla (simple o doble),
   un carácter especial (como redirecciones o pipes), o un token normal. 
   Dependiendo del tipo de carácter, llama a la función correspondiente para procesarlo.
   Retorna -1 si hay algún error (por ejemplo, si falta la comilla de cierre).
*/
int process_token(char *input, int *i, char **tokens, int *j)
{
	if (is_quote(input[*i]))                                                                 // Detecta si el carácter es una comilla (simple o doble)
	{
		*i = handle_quotes(input, *i, tokens, j);                                            // Maneja las comillas y obtiene el nuevo índice
		if (*i == -1)                                                                        // Verifica si hubo un error (por ejemplo, comilla no cerrada)
			return (-1);                                                                    
	}
	else if (is_special_char(input[*i]))                                                     // Detecta si el carácter es un carácter especial (|, <, >, etc.)
	{
		*i = handle_special_char(input, *i, tokens, j);                                      // Maneja los caracteres especiales
		if (*i == -1)                                                                        // Si hay un error, retorna -1
			return (-1);
	}
	else                                                                                     // Si no es comilla ni especial, es un token regular
	{
		*i = handle_token(input, *i, tokens, j);                                             // Procesa el token regular
		if (*i == -1)                                                                        // Verifica si hubo un error
			return (-1);
	}
	return 0;                                                                                // Retorna 0 si todo fue procesado correctamente
}

/*
   Esta es la función principal que realiza la tokenización de la cadena de entrada.
   - input: cadena de entrada del usuario.
   
   La función crea un array de tokens a partir de la entrada, manejando los espacios,
   comillas, redirecciones y caracteres especiales. Almacena los tokens en el array y 
   devuelve ese array para su posterior uso por el shell.

   Si ocurre algún error durante el proceso de tokenización (por ejemplo, falta una 
   comilla de cierre o no hay suficiente memoria), libera los recursos y retorna NULL.
*/
char	**tokenize_input(char *input)
{
	char	**tokens;                                                                       // Array de punteros para almacenar los tokens
	int		i;                                                                              // Índice para recorrer la cadena de entrada
	int		j;                                                                              // Índice para almacenar tokens en el array

	i = 0;                                                                                   // Inicializa el índice de entrada a 0
	j = 0;                                                                                   // Inicializa el índice de tokens a 0
	tokens = ft_calloc(100, sizeof(char *));                                                 // Reserva memoria para un máximo de 100 tokens
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
	tokens[j] = NULL;                                                                       // Marca el final del array de tokens con NULL
	return (tokens);                                                                        // Devuelve el array de tokens generado
}
// IMPLEMENTAR SI I = -1. LIBERAR TOKENS EN CADA ACCION.                                      // Verifica siempre si i=-1 para liberar los recursos apropiadamente en caso de error.
