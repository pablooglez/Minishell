/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albelope <albelope@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/11 19:28:02 by albelope          #+#    #+#             */
/*   Updated: 2024/11/16 19:55:02 by albelope         ###   ########.fr       */
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

void	replace_tabs_with_spaces(char *input)
{
	int	i;

	i = 0;
	while (input[i])
	{
		if (input[i] == '\t')
			input[i] = ' ';
		i++;
	}
}

int	remove_extra_spaces(char *input, char *norm)
{
	int	i;
	int	j;
	int	in_word;

	i = 0;
	j = 0;
	in_word = 0;
	while (input[i])
	{
		if (input[i] != ' ')
		{
			norm[j++] = input[i];
			in_word = 1;
		}
		else if (in_word)
		{
			norm[j++] = ' ';
			in_word = 0;
		}
		i++;
	}
	if (j > 0 && norm[j - 1] == ' ')
		j--;
	return (j);
}




