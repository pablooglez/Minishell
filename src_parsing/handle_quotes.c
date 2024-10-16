/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_quotes.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albelope <albelope@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/12 20:32:11 by albelope          #+#    #+#             */
/*   Updated: 2024/10/16 16:24:19 by albelope         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

/*sencilla. Solo revisa si un carácter es una comilla simple o comilla doble. 
 ayuda a identificar cuándo comienza y cuándo termina una cadena que está 
 entre comillas. Todo lo que este entre comillas se trata como un token.*/ 
bool	is_quote(char c)
{
	return (c == '\'' || c == '\"');
}
/*MANEJAMOS	los caracteres escapados dentro de las comillas, cuando algun mamon
mete un backslash en el input, significa que el seguiente char se trata literalmente.
ej. \" quiero que el backslash se ignore y que la comilla " se trate como parte
del texto y no como el cierre de una cadena.*/
int	handle_escape(char *input, int i, char *temp, int *k)
{
	i++; // Salta el backslash del input metido por algun mamon
    if (input[i])
	    temp[(*k)++] = input[i++];
	return (i);
}
/*Para gestionar todo lo que el usuario mete entre comillas, ya sea simples o dobles
para que se trate como un token.
Detectar si es comilla simple o doble, y la almacenamos en quoute_char. con i++ para
saber que hay en el next caracter dentro de las comillas.
Ejecuta el bluce hasta el final del input y no se haya encontrado comilla de cierre.
-- si encontramos backslash debe tratarse como habndle escape. SI no es un backslash, lo 
copiamos en el buffer temporal, incrementamos i y k.
-- si falta comillas de cierre. dveolvemos error
-- añadimos caracter nuelo para cerrar la cadena.
usamos ft_strdup para duplicar el contenido y guardarlo en tokens[*j], donde guardamos
los tokens, luego avanzamos j++ para guardar en la siguiente posicion disponible en el 
array de tokens.
devolvermos i + 1  que es la posicion en el input despues de la comilla de cierrte
*/

int handle_quotes(char *input, int i, char **tokens, int *j)
{
	char quote_char;
	char temp[1024]; // Un tamaño fijo para minishell mejor que dinámico? por tema de memoria depues(frees);
	int k;

	k = 0;
	quote_char = input[i++];
	while (input[i] && input[i] != quote_char)
	{
		if (input[i] == '\\')
			i = handle_escape(input, i, temp, &k);
		else
			temp[k++] = input[i++];
	}
	if (input[i] != quote_char)
		return (-1);
	temp[k] = '\0';
	tokens[*j] = ft_strdup(temp);
	(*j)++;
	return (i + 1);
}

