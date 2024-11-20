/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albelope <albelope@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/11 20:39:51 by albelope          #+#    #+#             */
/*   Updated: 2024/11/20 03:18:26 by albelope         ###   ########.fr       */
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

		// Procesar argumentos para el comando actual
		if (process_arguments(tokens, &i, current_cmd, shell) == -1)
		{
			//printf("[ERROR]-->PROCESS_TOKENS==> Error en process_arguments\n");
			return (-1);
		}

		// Verificar si hay más tokens y avanzar al siguiente comando
		if (tokens[i] && current_cmd->type == PIPE)
		{
			if (!current_cmd->next)
			{
				//printf("[ERROR]-->PROCESS_TOKENS==> current_cmd->next es NULL después de crear un nuevo comando\n");
				return (-1);
			}
			//printf("[DEBUG]-->PROCESS_TOKENS==> Avanzando al siguiente comando después de la pipe\n");
			current_cmd = current_cmd->next;
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

int expand_variable(char *input, int i, char *buffer, int *buf_index)
{
    char var_name[256];
    int var_len = 0;

    i++; // Skip the '$' character

    if (input[i] == '\0') // No variable name after '$'
        return i;

    // Extract the variable name
    while (input[i] && (ft_isalnum(input[i]) || input[i] == '_'))
    {
        var_name[var_len++] = input[i++];
    }
    var_name[var_len] = '\0';

    // Get the variable value using getenv
    char *value = getenv(var_name);
    if (value)
    {
        // Copy the value into the buffer
        int k = 0;
        while (value[k])
        {
            buffer[(*buf_index)++] = value[k++];
        }
    }
    // If the variable is not found, do nothing (empty string)

    return i;
}

int	handle_token(char *input, int i, char **tokens, int *j)
{
	char	buffer[1024];
	int		buf_index = 0;
	int		no_expand = 0;

	while (input[i] && input[i] != ' ')
	{
		if (input[i] == '\'') // Manejar comillas simples
		{
			//printf("[DEBUG]-->HANDLE_TOKEN-01==> Handling comillas simples\n");
			no_expand = 1; // Marcar como no expandible
			i++; // Saltar la comilla inicial
			while (input[i] && input[i] != '\'')
			{
				buffer[buf_index++] = input[i++];
			}
			if (input[i] != '\'') // Comillas sin cerrar
			{
				//printf("[ERROR]-->HANDLE_TOKEN==> Comillas simples sin cerrar\n");
				return (-1);
			}
			i++; // Saltar la comilla final
		}
		else if (input[i] == '"') // Manejar comillas dobles
		{
			//printf("[DEBUG]-->HANDLE_TOKEN-03==> Handling comillas dobles\n");
			i++; // Saltar la comilla inicial
			while (input[i] && input[i] != '"')
			{
				if (input[i] == '$') // Expandir variables
				{
					i = expand_variable(input, i, buffer, &buf_index);
					if (i == -1)
						return (-1); // Error durante la expansión
				}
				else
				{
					buffer[buf_index++] = input[i++];
				}
			}
			if (input[i] != '"') // Comillas sin cerrar
			{
				//printf("[ERROR]-->HANDLE_TOKEN==> Comillas dobles sin cerrar\n");
				return (-1);
			}
			i++; // Saltar la comilla final
		}
		else if (input[i] == '\\') // Manejar caracteres escapados
		{
			//printf("[DEBUG]-->HANDLE_TOKEN-04==> Handling carácter escapado\n");
			i = handle_escape(input, i, buffer, &buf_index);
			if (i == -1)
			{
				//printf("[ERROR]-->HANDLE_TOKEN==> Error al manejar carácter escapado\n");
				return (-1);
			}
		}
		else if (input[i] == '$') // Manejar variables fuera de comillas
		{
			i = expand_variable(input, i, buffer, &buf_index);
			if (i == -1)
			{
				//printf("[ERROR]-->HANDLE_TOKEN==> Error al expandir variable\n");
				return (-1);
			}
		}
		else // Agregar caracteres normales al buffer
		{
			buffer[buf_index++] = input[i++];
			//printf("[DEBUG]-->HANDLE_TOKEN-06==> Añadiendo carácter al buffer: [%c]\n", buffer[buf_index - 1]);
		}
	}

	buffer[buf_index] = '\0'; // Finalizar el buffer

	// Añadir el token al array de tokens
	if (buf_index > 0)
	{ 
		if (no_expand)
		{
			tokens[*j] = ft_strjoin("__NO_EXPAND__", buffer); // Token literal
			//printf("[DEBUG]-->HANDLE_TOKEN-08==> Token creado (sin expansión): [%s]\n", tokens[*j]);
		}
		else
		{
			tokens[*j] = ft_strdup(buffer); // Token normal
			//printf("[DEBUG]-->HANDLE_TOKEN-09==> Token creado: [%s]\n", tokens[*j]);
		}

		if (!tokens[*j]) // Validar duplicado
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
	//printf("****[DEBUG]-->TOKENIZE_INPUT==> Tokenización completada.		   [Tokens Creados: %d]\n", j);
	return (tokens);
}
