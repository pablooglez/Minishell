/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_char.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albelope <albelope@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/13 17:49:52 by albelope          #+#    #+#             */
/*   Updated: 2024/11/16 18:56:58 by albelope         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


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
		if (input[i] == '\0')
		{
			//printf("[DEBUG]-->CONTAINS_INVALID[0.1]==> Carácter nulo detected: [%c]\n", input[i]);
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
	//printf("[DEBUG]-->HANDLE_SPECIAL[0.1]==> Carácter especial detectado: [%c]\n", input[i]);
	if ((input[i] == '>' && input[i + 1] == '>') || (input[i] == '<' && input[i + 1] == '<'))
	{
		//printf("[DEBUG]-->HANDLE_SPECIAL[0.2]==> Redirección doble detectada: [%c%c]\n", input[i], input[i + 1]);
		temp = ft_substr(input, i, 2);
		if (!temp)
		{
			//printf("[ERROR]-->HANDLE_SPECIAL[0.3]==> Error al asignar memoria para redirección doble\n");
			return (-1);
		}
		tokens[*j] = temp;
		(*j)++;
		//printf("[DEBUG]-->HANDLE_SPECIAL[0.4]==> Token añadido (doble): [%s]\n", temp);
		return (i + 2);
	}
	else if (type != UNKNOWN)
	{
		//printf("[DEBUG]-->HANDLE_SPECIAL[0.5]==> Redirección simple detectada: [%c]\n", input[i]);
		temp = ft_substr(input, i, 1);
		if (!temp)
		{
			//printf("[ERROR]-->HANDLE_SPECIAL[0.6]==> Error al asignar memoria para redirección simple\n");
			return (-1);
		}
		tokens[*j] = temp;
		(*j)++;
		//printf("[DEBUG]-->HANDLE_SPECIAL[0.7]==> Token añadido (simple): [%s]\n", temp);
		return (i + 1);
	}
	//printf("[ERROR]-->HANDLE_SPECIAL[0.8]==> Carácter especial desconocido: [%c]\n", input[i]);
	return (-1);
}



