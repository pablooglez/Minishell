/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albelope <albelope@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/11 19:28:02 by albelope          #+#    #+#             */
/*   Updated: 2024/11/11 19:28:34 by albelope         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int is_empty_or_whitespace(char *str)
{
	if (!str)																												// Verifica si el string es NULL
		return (1);																											// Retorna 1 si está vacío
	while (*str)																											// Recorre cada carácter del string
	{
		if (*str != ' ')																									// Si encuentra un carácter diferente de espacio
			return (0);																										// Retorna 0 indicando que no está vacío
		str++;																												// Avanza al siguiente carácter
	}
	return (1);																												// Retorna 1 si todos los caracteres son espacios
}