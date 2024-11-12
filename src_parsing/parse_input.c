/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_input.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albelope <albelope@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/11 21:23:53 by albelope          #+#    #+#             */
/*   Updated: 2024/11/12 20:03:19 by albelope         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


/*int	process_arguments(char **tokens, int *i, t_cmd *cmd, t_minishell *shell)
{
	int	arg_index;
	int	ret;

	if (initialize_arguments(tokens, i, cmd) == -1)
		return (-1);
	arg_index = 0;
	cmd->arguments[arg_index] = ft_strdup(cmd->path);
	if (!cmd->arguments[arg_index])
		return (-1);
	arg_index++;
	while (tokens[*i] && ft_strncmp(tokens[*i], "|", 2) != 0)
	{
		if (get_redirection_type(tokens[*i]) != NOT_REDIR)
		{
			ret = process_redirection(tokens, i, cmd, shell);
			if (ret == -1)
				return (-1);
		}
		else
		{
			if (add_argument(tokens[*i], arg_index, cmd) == -1)
				return (-1);
			arg_index++;
			(*i)++;
		}
	}
	cmd->arguments[arg_index] = NULL;
	return (0);
}

int	process_token(char *input, int *i, char **tokens, int *j)
{
	if (get_special_token_type(input[*i]) != UNKNOWN)
	{
		*i = handle_special_char(input, *i, tokens, j);
		if (*i == -1)
			return (-1);
	}
	else
	{
		*i = handle_token(input, *i, tokens, j);
		if (*i == -1)
			return (-1);
	}
	return (0);
}*/
int	process_arguments(char **tokens, int *i, t_cmd *cmd, t_minishell *shell)
{
	int	arg_index;
	int	ret;

	printf("[DEBUG]-->PROCESS_ARGUMENTS[0.1]==> Init argumentos\n");

	if (initialize_arguments(tokens, i, cmd) == -1)
	{
		printf("[ERROR]-->PROCESS_ARGUMENTS[0.2]==> Error en initialize_arguments\n");
		return (-1);
	}

	arg_index = 0;
	cmd->arguments[arg_index] = ft_strdup(cmd->path);
	if (!cmd->arguments[arg_index])
	{
		printf("[ERROR]-->PROCESS_ARGUMENTS[0.3]==> Error al duplicar cmd->path\n");
		return (-1);
	}
	printf("[DEBUG]-->PROCESS_ARGUMENTS[0.4]==> Argumento[0] asignado:         [%s]\n", cmd->arguments[arg_index]);
	arg_index++;

	while (tokens[*i] && ft_strncmp(tokens[*i], "|", 2) != 0)
	{
		printf("[DEBUG]-->PROCESS_ARGUMENTS[0.5]==> Procesando token:              [%s]\n", tokens[*i]);


		if (get_redirection_type(tokens[*i]) != NOT_REDIR)
		{
			printf("[DEBUG]-->PROCESS_ARGUMENTS[0.6]==> Redirección detectada:               [%s]\n", tokens[*i]);
			ret = process_redirection(tokens, i, cmd, shell);
			if (ret == -1)
			{
				printf("[ERROR]-->PROCESS_ARGUMENTS[0.7]==> Error en process_redirection\n");
				return (-1);
			}
		}
		else
		{
		
			printf("[DEBUG]-->PROCESS_ARGUMENTS[0.8]==> Añadiendo argumento:           [%s]\n", tokens[*i]);
			if (add_argument(tokens[*i], arg_index, cmd) == -1)
			{
				printf("[ERROR]-->PROCESS_ARGUMENTS[0.9]==> Error en add_argument\n");
				return (-1);
			}
			arg_index++;
			(*i)++;
		}
	}


	cmd->arguments[arg_index] = NULL;
	printf("[DEBUG]-->PROCESS_ARGUMENTS[1.0]==> Fin Process. Total argumentos: [%d]\n", arg_index);
	return (0);
}
int	process_token(char *input, int *i, char **tokens, int *j)
{
	printf("[DEBUG]-->PROCESS_TOKEN[0.1]==> Init processs del token en input:  [%d]:[%c]\n", *i, input[*i]);


	if (get_special_token_type(input[*i]) != UNKNOWN)
	{
		printf("[DEBUG]-->PROCESS_TOKEN[0.2]==> Token especial detectado:                  [%c]\n", input[*i]);
		*i = handle_special_char(input, *i, tokens, j);
		if (*i == -1)
		{
			printf("[ERROR]-->PROCESS_TOKEN[0.3]==> Error en handle_special_char\n");
			return (-1);
		}
		printf("[DEBUG]-->PROCESS_TOKEN[0.4]==> Índice i después token especial:  [%d]\n", *i);
	}
	else
	{
		printf("[DEBUG]-->PROCESS_TOKEN[0.5]==> Token normal detectado:            [%c]\n", input[*i]);
		*i = handle_token(input, *i, tokens, j);
		if (*i == -1)
		{
			printf("[ERROR]-->PROCESS_TOKEN[0.6]==> Error en handle_token\n");
			return (-1);
		}
		printf("[DEBUG]-->PROCESS_TOKEN[0.7]==> Índice despues token normal:       [%d]\n", *i);
	}

	printf("[DEBUG]-->PROCESS_TOKEN[0.8]==> Token añadido al array de tokens:  [%s]\n", tokens[*j - 1]);
	return (0);
}


t_cmd	*parse_input(char *input_line, t_minishell *shell)
{
	char	**tokens;
	t_cmd	*cmd;
	t_cmd	*current_cmd;		// Variable para recorrer la lista de comandos de printf
	int		i = 0;				// Variable para recorrer el bucle de printf

	if (is_empty_or_whitespace(input_line))                                                // Verificar si la línea está vacía o tiene solo espacios
		return (NULL);
	if (contains_invalid_characters(input_line))                                           // Verificar si la línea contiene caracteres inválidos
		return (NULL);
	tokens = tokenize_input(input_line);
	if (!tokens || !tokens[0])
	{
		free_tokens_parse(tokens);
		return (NULL);
	}
	printf("[DEBUG]-->PARSE_INPUT[0.1]==> Tokens generados:\n");
	i = 0;
	while (tokens[i])
	{
		printf("[DEBUG]-->PARSE_INPUT[0.2]==> 	Token[%d]-->              ->        [%s]\n", i, tokens[i]);
		i++;
	}
	cmd = create_new_command(shell);
	if (!cmd)
	{
		free_tokens_parse(tokens);
		return (NULL);
	}
	/*if (tokens[0][0] == '$')
	{
		cmd->path = ft_strdup("echo");
		printf("[DEBUG]-->PARSE_INPUT[0.5]==> cmd->path asignado a 'echo':             [%s]\n", cmd->path);
	}*/
	i = 0;
	while (tokens[i])
	{
		printf("[DEBUG]-->PARSE_INPUT[0.7]==> Token[%d] antes de expandir:          [%s]\n", i, tokens[i]);
		i++;
	}
	if (process_tokens(tokens, cmd, shell) == -1)
	{
		free_tokens_parse(tokens);
		free_command(cmd);
		return (NULL);
	}
	expand_tokens(cmd, shell);
	printf("[DEBUG]-->PARSE_INPUT[0.8]==> Comandos creados y argumentos:\n");
	current_cmd = cmd;
	while (current_cmd)
	{
		printf("[DEBUG]-->PARSE_INPUT[0.9]==> Comando (cmd->path):                 [%s]\n", current_cmd->path);
		i = 0;
		while (current_cmd->arguments && current_cmd->arguments[i])
		{
			printf("[DEBUG]-->PARSE_INPUT[0.9.%d]==> Argumento[%d]:                      [%s]\n", i + 1, i, current_cmd->arguments[i]);
			i++;
		}
		current_cmd = current_cmd->next;
	}
	free_tokens_parse(tokens);
	return (cmd);
}

