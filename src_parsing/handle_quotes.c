/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_quotes.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pabloglez <pabloglez@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/12 20:32:11 by albelope          #+#    #+#             */
/*   Updated: 2024/11/19 16:40:18 by pabloglez        ###   ########.fr       */
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
	i++;
	while (input[i] && input[i] != '\'')
		buffer[(*buf_index)++] = input[i++];
	if (input[i] != '\'')
		return (error_handler("Minishell: syntax error\n", SYNTAX_ERROR));
	return (i + 1);
}

int	handle_double_quotes(char *input, int i, char *buffer, int *buf_index)
{
	i++;
	while (input[i] && input[i] != '"')
	{
		if (input[i] == '\\' && (input[i + 1] == '"' || input[i + 1] == '\\'))
		{
			i++;
			buffer[(*buf_index)++] = input[i++];
		}
		else
			buffer[(*buf_index)++] = input[i++];
	}
	if (input[i] != '"')
		return (error_handler("Minishell: syntax error\n", SYNTAX_ERROR));
	return (i + 1);
}
