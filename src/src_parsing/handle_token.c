/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_token.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albelope <albelope@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/29 00:20:12 by pablogon          #+#    #+#             */
/*   Updated: 2024/11/29 22:45:13 by albelope         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	s_q(int *no_expand, char *buffer,
		int *buf_index, t_minishell *shell)
{
	*no_expand = 1;
	shell->i++;
	while (shell->input[shell->i] && shell->input[shell->i] != '\'')
		buffer[(*buf_index)++] = shell->input[shell->i++];
	if (shell->input[shell->i] != '\'')
		return (-1);
	shell->i++;
	return (1);
}

static int	qts(int *no_expand, char *buffer,
		int *buf_index, t_minishell *shell)
{
	if (shell->input[shell->i] == '\'')
		return (s_q(no_expand, buffer, buf_index, shell));
	else if (shell->input[shell->i] == '"')
	{
		shell->i++;
		while (shell->input[shell->i] && shell->input[shell->i] != '"')
		{
			if (shell->input[shell->i] == '$')
			{
				shell->i = expand_variable(shell->input, buffer,
						buf_index, shell);
				if (shell->i == -1)
					return (-1);
			}
			else
				buffer[(*buf_index)++] = shell->input[shell->i++];
		}
		if (shell->input[shell->i] != '"')
			return (-1);
		shell->i++;
		return (1);
	}
	return (0);
}

int	spec(char **tokens, char *buffer,
			int *buf_index, t_minishell *shell)
{
	if (*buf_index > 0)
	{
		buffer[*buf_index] = '\0';
		tokens[shell->j] = ft_strdup(buffer);
		if (!tokens[shell->j])
		{
			free_tokens_parse(tokens);
			return (-1);
		}
		(shell->j)++;
		*buf_index = 0;
	}
	tokens[shell->j] = ft_substr(shell->input, shell->i, 1);
	if (!tokens[shell->j])
	{
		free_tokens_parse(tokens);
		return (-1);
	}
	shell->j++;
	shell->i++;
	return (shell->i);
}

static int	fin(char **tokens, char *buffer, int *buf_index, t_minishell *shell)
{
	buffer[*buf_index] = '\0';
	if (*buf_index > 0)
	{
		if (shell->e)
			tokens[shell->j] = ft_strjoin("__NO_EXPAND__", buffer);
		else
			tokens[shell->j] = ft_strdup(buffer);
		if (!tokens[shell->j])
		{
			free_tokens_parse(tokens);
			return (-1);
		}
		shell->j++;
	}
	return (shell->i);
}

int	handle_token(char **tokens, t_minishell *shell)
{
	char	buffer[1024];
	int		buf_index;
	int		no_expand;
	int		result;

	buf_index = 0;
	no_expand = 0;
	while (shell->input[shell->i] && shell->input[shell->i] != ' ')
	{
		shell->e = qts(&no_expand, buffer, &buf_index, shell);
		result = process_input_character(tokens, buffer, &buf_index, shell);
		if (result != 0)
			return (result);
	}
	shell->e = no_expand;
	return (fin(tokens, buffer, &buf_index, shell));
}
