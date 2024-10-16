/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albelope <albelope@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/11 20:39:51 by albelope          #+#    #+#             */
/*   Updated: 2024/10/16 16:38:53 by albelope         ###   ########.fr       */
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
void	free_tokens(char **tokens)
{
	int i;
	
	i = 0;
	if (!tokens)
		return;
	while (tokens[i])
	{
		free(tokens[i]);
		i++;
	}
	free(tokens);
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
	int start;

	start = i;
	while (input[i] && input[i] != ' ' && !is_quote(input[i]) && !is_special_char(input[i]))
		i++;
	tokens[*j] = ft_substr(input, start, i - start);
	if (!tokens[*j])
	{
		free_tokens(tokens);
		return (-1);
	}
	(*j)++;
	return (i);
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
    if (is_quote(input[*i]))
    {
        *i = handle_quotes(input, *i, tokens, j);
        if (*i == -1)
        {
            printf("Error: comilla no cerrada.\n");
            return (-1);
        }
    }
    else if (is_special_char(input[*i])) 
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
    return 0;
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
    char	**tokens;
	int		i;
	int 	j;

	i = 0;
	j = 0;
	tokens = ft_calloc(100, sizeof(char *));  // Suponemos un máximo de 100 tokens
	if (!tokens)
        return NULL;
    while (input[i])
    {
        while (input[i] == ' ')
            i++;
        if (process_token(input, &i, tokens, &j) == -1)
        {
            free_tokens(tokens);
            return NULL;
        }
        while (input[i] == ' ')
            i++;
    }
    tokens[j] = NULL;
    return (tokens);
}
 // IMPLEMENTAR SI I = -1. LIBERAR TOKENS EN CADA ACCION.
