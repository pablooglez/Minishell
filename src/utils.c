/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pabloglez <pabloglez@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/15 18:06:01 by pabloglez         #+#    #+#             */
/*   Updated: 2024/10/15 20:48:20 by pabloglez        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char *ft_strncpy(char *dest, const char *src, size_t n)
{
	size_t i = 0;									// Inicializa el índice a 0

													// Copiar hasta n caracteres o hasta que se encuentre un carácter nulo en src
	while (i < n && src[i] != '\0')					// Mientras no hay más caracteres en src y no hemos copiado n caracteres
	{
		dest[i] = src[i];							// Copia el carácter de src a dest
		i++;										// Incrementa el índice
	}

													// Si hemos copiado menos de n caracteres, rellenar con ceros nulos
	while (i < n)									// Mientras i sea menor que n
	{
		dest[i] = '\0';								// Asegura que el resto de dest esté terminado en nulo
		i++;										// Incrementa el índice
	}
	return dest;									// Retorna el puntero a la cadena destino
}


char *ft_strndup(const char *s, size_t n)
{
	size_t len = ft_strlen(s);						// Obtiene la longitud de la cadena original
	if (len > n)								// Si la longitud es mayor que n, ajusta la longitud a n
		len = n;

	char *dup = (char *)malloc(len + 1);		// Asigna memoria para la nueva cadena (incluyendo el carácter nulo)
	if (!dup)									// Si la asignación de memoria falla
		return NULL;							// Retorna NULL

												// Copia los caracteres de la cadena original a la nueva cadena
	ft_strncpy(dup, s, len);						// Copia hasta n caracteres
	dup[len] = '\0';							// Asegúrate de que la cadena esté terminada con un carácter nulo

	return dup;									// Retorna la nueva cadena duplicada
}
