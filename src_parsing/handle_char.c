/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_char.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albelope <albelope@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/13 17:49:52 by albelope          #+#    #+#             */
/*   Updated: 2024/10/30 20:08:50 by albelope         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

t_token	get_special_token_type(char c)
{
	if (c == '|')
		return (PIPE);
	if (c == '>')
		return (REDIR);
	if (c == '<')
		return (REDIR);
	return (UNKNOWN);
}

bool	contains_invalid_characters(char *input)
{
	int	i;

	i = 0;
	while (input[i])
	{
		if (input[i] == ';' || (input[i] == '\\' && input[i + 1] != '$'))
		{
			printf("Error: uso de caracteres especiales no permitidos (%c).\n", input[i]);
			return (true);
		}
		i++;
	}
	return (false);
}
int	handle_special_char(char *input, int i, char **tokens, int *j)
{
	char	*temp;
	t_token	type;

	type = get_special_token_type(input[i]);
	temp = NULL;
	if ((input[i] == '>' && input[i + 1] == '>') || (input[i] == '<' && input[i + 1] == '<'))
	{
		temp = ft_substr(input, i, 2);
		if (!temp)
			return (-1);
		tokens[*j] = temp;
		(*j)++;
		return (i + 2);
	}
	else if (type != UNKNOWN)
	{
		temp = ft_substr(input, i, 1);
		if (!temp)
			return (-1);
		tokens[*j] = temp;
		(*j)++;
		return (i + 1);
	}
	return (-1);
}
