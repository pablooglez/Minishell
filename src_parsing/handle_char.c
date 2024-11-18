/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_char.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albelope <albelope@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/13 17:49:52 by albelope          #+#    #+#             */
/*   Updated: 2024/11/18 18:02:25 by albelope         ###   ########.fr       */
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

int	is_unimplemented_operator(char *input, int i, int in_single_quotes, int in_double_quotes)
{
	if (!in_single_quotes && !in_double_quotes &&
		((input[i] == '&' && input[i + 1] == '&') || (input[i] == '|' && input[i + 1] == '|')))
	{
		if (input[i] == '&')
			print_error("Minishell: syntax error logic operator not implemented `&&'\n");
		else
			print_error("Minishell: syntax error logic operator not implemented `||'\n");
		return (1);
	}
	return (0);
}

int	is_invalid_token_sequence(char *input, int i, int in_single_quotes, int in_double_quotes)
{
	if (!in_single_quotes && !in_double_quotes &&
		((input[i] == ';' && input[i + 1] == ';') ||
		(input[i] == '>' && input[i + 1] == '>' && input[i + 2] == '>') ||
		(input[i] == '<' && input[i + 1] == '<' && input[i + 2] == '<')))
	{
		if (input[i] == ';')
			print_error("Minishell: syntax error near unexpected token `;;'\n");
		else if (input[i] == '>')
			print_error("Minishell: syntax error near unexpected token `>>>'\n");
		else
			print_error("Minishell: syntax error near unexpected token `<<<'\n");
		return (1);
	}
	return (0);
}

int	contains_invalid_characters(char *input)
{
	int	i;
	int	in_single_quotes = 0;
	int	in_double_quotes = 0;

	i = 0;
	while (input[i])
	{
		if (input[i] == '\'' && !in_double_quotes)
			in_single_quotes = !in_single_quotes;
		else if (input[i] == '\"' && !in_single_quotes)
			in_double_quotes = !in_double_quotes;
		if (is_unimplemented_operator(input, i, in_single_quotes, in_double_quotes) ||
			is_invalid_token_sequence(input, i, in_single_quotes, in_double_quotes))
			return (SYNTAX_ERROR);

		i++;
	}
	return (0);
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



