/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_quotes.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albelope <albelope@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/12 20:32:11 by albelope          #+#    #+#             */
/*   Updated: 2024/11/12 16:47:34 by albelope         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	is_quote(char c)
{
	return (c == '\'' || c == '\"');
}

bool	has_dollar_sign(const char *input, int start, int end)
{
	while (start < end)
	{
		if (input[start] == '$')
			return (true);
		start++;
	}
	return (false);
}

int	handle_single_quotes(char *input, int i, char *buffer, int *buf_index)
{
	printf("[DEBUG]-->H_SINGLE_QUOTES[0.1]==> Handling comillas simples\n");
	i++;
	while (input[i] && input[i] != '\'')
	{
		printf("[DEBUG]-->H_SINGLE_QUOTES[0.2]==> Añadiendo carácter al buffer:    [%c]\n", input[i]);
		buffer[(*buf_index)++] = input[i++];
	}
	if (input[i] != '\'')
	{
		printf("[ERROR]-->H_SINGLE_QUOTES[0.3]==> Error: comilla simple sin cerrar\n");
		return (-1);
	}
	printf("[DEBUG]-->H_SINGLE_QUOTES[0.4]==> End single_quotes Índice :       [%d]\n", i + 1);
	return (i + 1);
}

int	handle_double_quotes(char *input, int i, char *buffer, int *buf_index)
{
	printf("[DEBUG]-->H_DOUBLE_QUOTES[0.1]==> Handling comillas dobles\n");
	i++;
	while (input[i] && input[i] != '"')
	{
		if (input[i] == '\\' && (input[i + 1] == '"' || input[i + 1] == '\\'))
		{
			printf("[DEBUG]-->h_DOUBLE_QUOTES[0.2]==> Carácter escapado detectado:       [%c]\n", input[i + 1]);
			i++;
			buffer[(*buf_index)++] = input[i++];
		}
		else
		{
			printf("[DEBUG]-->H_DOUBLE_QUOTES[0.3]==> Añadiendo carácter al buffer:    [%c]\n", input[i]);
			buffer[(*buf_index)++] = input[i++];
		}
	}
	if (input[i] != '"')
	{
		printf("[ERROR]-->H_DOUBLE_QUOTES[0.4]==> Error: comilla doble sin cerrar\n");
		return (-1);
	}
	printf("[DEBUG]-->_DOUBLE_QUOTES[0.5]==> DOuble_quotes ok. Indice es:      [%d]\n", i + 1);
	return (i + 1);
}
