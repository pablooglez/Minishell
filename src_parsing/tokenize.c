/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pabloglez <pabloglez@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/11 20:39:51 by albelope          #+#    #+#             */
/*   Updated: 2024/11/19 16:49:47 by pabloglez        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


t_token	classify_special_token(char c)
{
	if (c == '|')																			// Verifica si el carcates es un Pipe
		return (PIPE);																		// Retorna PIPE si el carácter es un Pipe
	if (c == '>')																			// Verifica si el Caracter es una Redireccion
		return (REDIR);
	if (c == '<')
		return (REDIR);
	return (UNKNOWN);
}

int	process_tokens(char **tokens, t_cmd *current_cmd, t_minishell *shell)
{
	int	i = 0;
	
	while (tokens[i])
	{
		if (process_token_pipe(tokens, &i, &current_cmd, shell) == -1)
			return (-1);

		// Procesar argumentos para el comando actual
		if (process_arguments(tokens, &i, current_cmd, shell) == -1)
			return (-1);

		// Verificar si hay más tokens y avanzar al siguiente comando
		if (tokens[i] && current_cmd->type == PIPE)
		{
			if (!current_cmd->next)
				return (-1);
			current_cmd = current_cmd->next;
		}
	}
	return (0);
}

int	handle_escape(char *input, int i, char *buffer, int *buf_index)
{
	i++;
	if (input[i] == '"' || input[i] == '\'' || input[i] == '\\')
		buffer[(*buf_index)++] = input[i++];
	else
	{
		buffer[(*buf_index)++] = '\\';
		buffer[(*buf_index)++] = input[i++];
	}
	return (i);
}

int	handle_token(char *input, int i, char **tokens, int *j)
{
	char	buffer[1024];
	int		buf_index = 0;
	int		no_expand = 0;

	while (input[i] && input[i] != ' ')
	{
		if (input[i] == '\'')
		{
			no_expand = 1;
			i = handle_single_quotes(input, i, buffer, &buf_index);
		}
		else if (input[i] == '"')
			i = handle_double_quotes(input, i, buffer, &buf_index);
		else if (input[i] == '\\')
			i = handle_escape(input, i, buffer, &buf_index);
		else
			buffer[buf_index++] = input[i++];
		if (i == -1)
			return (-1);
	}
	buffer[buf_index] = '\0';
	if (buf_index > 0)
	{ 
		if (no_expand)
			tokens[*j] = ft_strjoin("__NO_EXPAND__", buffer);
		else
			tokens[*j] = ft_strdup(buffer);
		if (!tokens[*j])
		{
			free_tokens_parse(tokens);
			return (-1);
		}
		(*j)++;
	}
	return (i);
}

char	**tokenize_input(char *input)
{
	char	**tokens;
	int		i = 0;
	int		j = 0;

	tokens = ft_calloc(100, sizeof(char *));
	if (!tokens)
		return (NULL);

	while (input[i])
	{
		while (input[i] == ' ')
			i++;
		if (process_token(input, &i, tokens, &j) == -1)
		{
			free_tokens_parse(tokens);
			return (NULL);
		}
	}
	tokens[j] = NULL;
	return (tokens);
}
