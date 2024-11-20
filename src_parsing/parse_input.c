/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_input.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albelope <albelope@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/11 21:23:53 by albelope          #+#    #+#             */
/*   Updated: 2024/11/20 03:27:48 by albelope         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	process_arguments(char **tokens, int *i, t_cmd *cmd, t_minishell *shell)
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
}

t_cmd	*parse_input(char *input_line, t_minishell *shell)
{
	char	**tokens;
	t_cmd	*cmd;
	t_cmd	*current_cmd;																	// Variable para recorrer la lista de comandos de printf
	int		i = 0;																			// Variable para recorrer el bucle de printf

	if (is_empty_or_whitespace(input_line))                                                // Verificar si la línea está vacía o tiene solo espacios
		return (NULL);
	if (contains_invalid_characters(input_line))                                           // Verificar si la línea contiene caracteres inválidos
		return (NULL);
	//printf("[DEBUG]-->PARSE_INPUT[0.0]==> Iniciando parseo de la entrada es: 	   [%s]\n", input_line);
	tokens = tokenize_input(input_line);
	if (!tokens || !tokens[0])
	{
		free_tokens_parse(tokens);
		return (NULL);
	}
	//printf("*****[DEBUG]-->PARSE_INPUT[0.1]==> Tokens generados:\n");
	i = 0;
	while (tokens[i])
		i++;
	cmd = create_new_command(shell);
	//printf("[DEBUG]-->PARSE_INPUT[0.3]==> Comando creado:                 [%p]\n", (void *)cmd);
	//printf("[DEBUG]-->PARSE_INPUT[0.4]==> Comando creado (cmd->path):     [%s]\n", cmd->path);
	//printf("[DEBUG]-->PARSE_INPUT[0.5]==> Comando creado (cmd->type):     [%d]\n", cmd->type);
	//printf("[DEBUG]-->PARSE_INPUT[0.6]==> Comando creado (cmd->arguments): [%p]\n", (void *)cmd->arguments);
	if (!cmd)
	{
		free_tokens_parse(tokens);
		return (NULL);
	}
	/*if (tokens[0][0] == '$')
	{
		cmd->path = ft_strdup("echo");
		//printf("[DEBUG]-->PARSE_INPUT[0.5]==> cmd->path asignado a 'echo':             [%s]\n", cmd->path);
	}*/
	i = 0;
	while (tokens[i])
	{
		//printf("***[DEBUG]-->PARSE_INPUT[0.7]==> Token[%d] antes de expandir:          [%s]\n", i, tokens[i]);
		i++;
	if (process_tokens(tokens, cmd, shell) == -1)
	{
		free_tokens_parse(tokens);
		free_command(cmd);
		return (NULL);
	}
	expand_tokens(cmd, shell);
	//printf("***[DEBUG]-->PARSE_INPUT[0.8]==> Comandos creados y argumentos:\n");
	current_cmd = cmd;
	while (current_cmd)
	{
		i = 0;
		while (current_cmd->arguments && current_cmd->arguments[i])
			i++;
		current_cmd = current_cmd->next;
	}
	free_tokens_parse(tokens);
	return (cmd);
}

