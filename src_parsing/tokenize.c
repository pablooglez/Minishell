/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albelope <albelope@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/11 20:39:51 by albelope          #+#    #+#             */
/*   Updated: 2024/11/16 18:56:58 by albelope         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


t_token	classify_special_token(char c)
{
	if (c == '|')																			// Verifica si el carcates es un Pipe
	{
		//printf("[DEBUG]-->CLASSIFY_SPECIAL_TOKEN==> Token especial detectado: PIPE\n");
		return (PIPE);																		// Retorna PIPE si el carácter es un Pipe
	}
	if (c == '>')																			// Verifica si el Caracter es una Redireccion	
	{
		//printf("[DEBUG]-->CLASSIFY_SPECIAL_TOKEN==> Token especial detectado: REDIR (>)\n");
		return (REDIR);
	}
	if (c == '<')
	{
		//printf("[DEBUG]-->CLASSIFY_SPECIAL_TOKEN==> Token especial detectado: REDIR (<)\n");
		return (REDIR);
	}
	return (UNKNOWN);
}

int	process_tokens(char **tokens, t_cmd *current_cmd, t_minishell *shell)
{
	int	i = 0;

	//printf("[DEBUG]-->PROCESS_TOKENS==> Iniciando procesamiento de tokens\n");
	while (tokens[i])
	{
		if (process_token_pipe(tokens, &i, &current_cmd, shell) == -1)
		{
			//printf("[ERROR]-->PROCESS_TOKENS==> Error en process_token_pipe\n");
			return (-1);
		}
		if (process_arguments(tokens, &i, current_cmd, shell) == -1)
		{
			//printf("[ERROR]-->PROCESS_TOKENS==> Error en process_arguments\n");
			return (-1);
		}
	}
	//printf("[DEBUG]-->PROCESS_TOKENS==> Finalizando procesamiento de tokens\n");
	return (0);
}

int	handle_escape(char *input, int i, char *buffer, int *buf_index)
{
	i++;
	if (input[i] == '"' || input[i] == '\'' || input[i] == '\\')
	{
		//printf("[DEBUG]-->HANDLE_ESCAPE==> Carácter escapado detectado: [%c]\n", input[i]);
		buffer[(*buf_index)++] = input[i++];
	}
	else
	{
		//printf("[DEBUG]-->HANDLE_ESCAPE==> Carácter no escapado: [%c]\n", input[i]);
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
			//printf("[DEBUG]-->HANDLE_TOKEN-01==> Handling comillas simples\n");
			no_expand = 1;
			i = handle_single_quotes(input, i, buffer, &buf_index);
			//printf("[DEBUG]-->HANDLE_TOKEN-02==> End single_quotes Índice :       	   [%d]\n", i);
		}
		else if (input[i] == '"')
		{
			//printf("[DEBUG]-->HANDLE_TOKEN-03==> Handling comillas dobles\n");
			i = handle_double_quotes(input, i, buffer, &buf_index);
			//printf("[DEBUG]-->HANDLE_TOKEN==> End double_quotes Índice :       	[%d]\n", i);
		}
		else if (input[i] == '\\')
		{
			//printf("[DEBUG]-->HANDLE_TOKEN-04==> Handling carácter escapado\n");
			i = handle_escape(input, i, buffer, &buf_index);
			//printf("[DEBUG]-->HANDLE_TOKEN-05==> End escape Índice :      				[%d]\n", i);
		}
		else
		{
			buffer[buf_index++] = input[i++];
			//printf("[DEBUG]-->HANDLE_TOKEN-06==> Añadiendo carácter al buffer:         [%c]\n", buffer[buf_index - 1]);
		}

		if (i == -1)
		{
			//printf("[ERROR]-->HANDLE_TOKEN-07==> Error al manejar el token\n");
			return (-1);
		}
	}

	buffer[buf_index] = '\0';
	if (buf_index > 0)
	{ 
		if (no_expand)
		{
			tokens[*j] = ft_strjoin("__NO_EXPAND__", buffer);
			//printf("[DEBUG]-->HANDLE_TOKEN-08==> Token creado (sin expansión): 	   [%s]\n", tokens[*j]);
		}
		else
		{
			tokens[*j] = ft_strdup(buffer);
			//printf("[DEBUG]-->HANDLE_TOKEN-09==> Token creado:			   [%s]\n", tokens[*j]);
		}

		if (!tokens[*j])
		{
			//printf("[ERROR]-->HANDLE_TOKEN-10==> Error al duplicar el token\n");
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

	//printf("[DEBUG]-->TOKENIZE_INPUT==> Iniciando tokenización\n");
	tokens = ft_calloc(100, sizeof(char *));
	if (!tokens)
	{
		//printf("[ERROR]-->TOKENIZE_INPUT==> Error al asignar memoria para tokens\n");
		return (NULL);
	}

	while (input[i])
	{
		while (input[i] == ' ')
			i++;
		if (process_token(input, &i, tokens, &j) == -1)
		{
			//printf("[ERROR]-->TOKENIZE_INPUT==> Error en process_token\n");
			free_tokens_parse(tokens);
			return (NULL);
		}
	}

	tokens[j] = NULL;
	//printf("[DEBUG]-->TOKENIZE_INPUT==> Tokenización completada.		   [Tokens Creados: %d]\n", j);
	return (tokens);
}
