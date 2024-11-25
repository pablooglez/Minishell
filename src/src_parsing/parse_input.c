/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_input.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albelope <albelope@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/11 21:23:53 by albelope          #+#    #+#             */
/*   Updated: 2024/11/25 13:09:43 by albelope         ###   ########.fr       */
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

	if (is_empty_or_whitespace(input_line)) 
		return (NULL);
	if (contains_invalid_characters(input_line)) 
		return (NULL);
	tokens = tokenize_input(input_line);
	if (!tokens || !tokens[0])
	{
		free_tokens_parse(tokens);
		return (NULL);
	}
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

