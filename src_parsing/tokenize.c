/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albelope <albelope@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/11 20:39:51 by albelope          #+#    #+#             */
/*   Updated: 2024/11/20 20:30:52 by albelope         ###   ########.fr       */
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
		if (process_arguments(tokens, &i, current_cmd, shell) == -1)
			return (-1);
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

int	expand_variable(char *input, int i, char *buffer, int *buf_index)
{
	char	var_name[256];
	int		var_len;
	char	*pid;
	int		k;
	char	*value;

	var_len = 0;
	i++; 
	if (input[i] == '\0' || input[i] == ' ')
	{
		buffer[(*buf_index)++] = '$';
		return (i);
	}
	if (input[i] == '$')
	{
		i++;
		pid = ft_itoa(getpid());
		k = 0;
		while (pid[k])
			buffer[(*buf_index)++] = pid[k++];
		free(pid);
		return (i);
	}
	while (input[i] && (ft_isalnum(input[i]) || input[i] == '_'))
		var_name[var_len++] = input[i++];
	var_name[var_len] = '\0';
	value = getenv(var_name);
	if (value)
	{
		k = 0;
		while (value[k])
			buffer[(*buf_index)++] = value[k++];
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
			i++; 
			while (input[i] && input[i] != '\'')
			{
				buffer[buf_index++] = input[i++];
			}
			if (input[i] != '\'')
			{
				//printf("[ERROR]-->HANDLE_TOKEN==> Comillas simples sin cerrar\n");
				return (-1);
			}
			i++;
		}
		else if (input[i] == '"')
		{
			//printf("[DEBUG]-->HANDLE_TOKEN-03==> Handling comillas dobles\n");
			i++;
			while (input[i] && input[i] != '"')
			{
				if (input[i] == '$') 
				{
					i = expand_variable(input, i, buffer, &buf_index);
					if (i == -1)
						return (-1);
				}
				else
				{
					buffer[buf_index++] = input[i++];
				}
			}
			if (input[i] != '"') 
			{
				//printf("[ERROR]-->HANDLE_TOKEN==> Comillas dobles sin cerrar\n");
				return (-1);
			}
			i++; 
		}
		else if (input[i] == '\\')
		{
			//printf("[DEBUG]-->HANDLE_TOKEN-04==> Handling carácter escapado\n");
			i = handle_escape(input, i, buffer, &buf_index);
			if (i == -1)
			{
				//printf("[ERROR]-->HANDLE_TOKEN==> Error al manejar carácter escapado\n");
				return (-1);
			}
		}
		else if (input[i] == '$')
		{
			i = expand_variable(input, i, buffer, &buf_index);
			if (i == -1)
			{
				//printf("[ERROR]-->HANDLE_TOKEN==> Error al expandir variable\n");
				return (-1);
			}
		}
		else
		{
			buffer[buf_index++] = input[i++];
			//printf("[DEBUG]-->HANDLE_TOKEN-06==> Añadiendo carácter al buffer: [%c]\n", buffer[buf_index - 1]);
		}
	}
	buffer[buf_index] = '\0';
	if (buf_index > 0)
	{ 
		if (no_expand)
		{
			tokens[*j] = ft_strjoin("__NO_EXPAND__", buffer);
			//printf("[DEBUG]-->HANDLE_TOKEN-08==> Token creado (sin expansión): [%s]\n", tokens[*j]);
		}
		else
		{
			tokens[*j] = ft_strdup(buffer);
			//printf("[DEBUG]-->HANDLE_TOKEN-09==> Token creado: [%s]\n", tokens[*j]);
		}
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


	//printf("[DEBUG]-->TOKENIZE_INPUT==> Iniciando tokenización\n");
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
	//printf("****[DEBUG]-->TOKENIZE_INPUT==> Tokenización completada.		   [Tokens Creados: %d]\n", j);
	return (tokens);
}
