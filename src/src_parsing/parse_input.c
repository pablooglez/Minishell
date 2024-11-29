/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_input.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albelope <albelope@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/11 21:23:53 by albelope          #+#    #+#             */
/*   Updated: 2024/11/29 20:04:12 by albelope         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	process_token(char *input, char **tokens, int *j, t_minishell *shell)
{
	if (get_special_token_type(input[shell->i]) != UNKNOWN)
	{
		shell->i = handle_special_char(input, shell->i, tokens, j);
		if (shell->i == -1)
			return (-1);
	}
	else
	{
		shell->input = input;
		shell->j = *j;
		shell->i = handle_token(tokens, shell);
		*j = shell->j;
		if (shell->i == -1)
			return (-1);
	}
	return (0);
}

int	validate_input(char *input)
{
	if (is_empty_or_whitespace(input))
		return (0);
	if (contains_invalid_characters(input))
		return (0);
	return (1);
}

char	**validate_and_tokenize(char *input_line, t_minishell *shell)
{
	char	**tokens;

	tokens = tokenize_input(input_line, shell);
	if (!tokens || !tokens[0])
	{
		free_tokens_parse(tokens);
		return (NULL);
	}
	return (tokens);
}

t_cmd	*parse_input(char *input_line, t_minishell *shell)
{
	char	**tokens;
	t_cmd	*cmd;

	if (!validate_input(input_line))
		return (NULL);
	tokens = validate_and_tokenize(input_line, shell);
	if (!tokens)
		return (NULL);
	cmd = create_new_command(shell);
	if (!cmd)
	{
		free_tokens_parse(tokens);
		return (NULL);
	}
	if (process_tokens(tokens, cmd, shell) == -1)
	{
		free_tokens_parse(tokens);
		free_command(cmd);
		return (NULL);
	}
	expand_tokens(cmd, shell);
	free_tokens_parse(tokens);
	return (cmd);
}
