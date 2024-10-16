/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_char.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albelope <albelope@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/13 17:49:52 by albelope          #+#    #+#             */
/*   Updated: 2024/10/14 13:48:35 by albelope         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

/*VERIFICAR SI EL CARACTER ES UN CARACTER ESPECIAL (PIPES, REDIRECCIONES, ETC.)
	EN LA SINTAXIS DE SHELL (MINISHELL), */

int	is_special_char(char c)
{
	return (c == '|' || c == '>' || c == '<' || c == ';');
}

/* MANEJAR LA TOKENIZACION DE CARACTERES ESPECIALES-- INCLUYENDO REDIRECCIONES
	SIMPLES Y DOBLES.
	Recibe la cadena que el usuario introduce (input)-- (i) indice actual de la
	cadena input-- (**tokens) array donde almacenaremos los tokens generados
	a partir de la cadena de entrada -- (*j) indice actual en el array de tokens.
	(en que posicion vamos a almacenar el siguiente token.)
	Puntero Temporal para almacenar temp el token q representa el caracter especial.
	Verificamos (if) si es redireccion doble o simple. Extraemos 1 caracter o 2, con
	ft_substr. Almacenamos el contenido de temp en la posicion actual del array.....
    ... de tokens (tokens[*j]). Avanzamos *j++ para asegurar que el siguiente token...
    .. se almacene en la proxima posicion vacia del array... 
    Devuelve i + 1 o i + 2, dependiendo si es redireccion simple o doble. y saltar
    el caracter que ya hemos procesado.*/
int handle_special_char(char *input, int i, char **tokens, int *j)
{
    char	*temp;

    temp = NULL;
    if ((input[i] == '>' && input[i + 1] == '>') || (input[i] == '<' && input[i + 1] == '<'))
    {
        temp = ft_substr(input, i, 2);
        if (!temp)
        {
            free_tokens(tokens);
            return (-1);
        }
        tokens[*j] = temp;
        (*j)++;
        return (i + 2);
    }
    temp = ft_substr(input, i, 1);
    if (!temp)
    {
        free_tokens(tokens);
        return (-1);
    }
    tokens[*j] = temp;
    (*j)++;
    return (i + 1);
}
