/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albelope <albelope@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/11 20:39:51 by albelope          #+#    #+#             */
/*   Updated: 2024/12/01 13:17:53 by albelope         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_token	classify_special_token(char c)
{
	if (c == '|')
		return (PIPE);
	if (c == '>')
		return (REDIR);
	if (c == '<')
		return (REDIR);
	return (UNKNOWN);
}

int	process_tokens(char **tokens, t_cmd *current_cmd, t_minishell *shell)
{
	int	i;
	int	redir_type;

	i = 0;
	while (tokens[i])
	{
		redir_type = get_redirection_type(tokens[i]);
		if (redir_type != NOT_REDIR
			&& process_redirection(tokens, &i, current_cmd, shell) == -1)
			return (-1);
		else if (tokens[i] && ft_strncmp(tokens[i], "|", 1) == 0
			&& process_token_pipe(tokens, &i, &current_cmd, shell) == -1)
			return (-1);
		else if (redir_type == NOT_REDIR
			&& process_arguments(tokens, &i, current_cmd, shell) == -1)
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

int	expand_variable(char *input, char *buffer, int *buf_index,
	t_minishell *shell)
{
	char	var_name[256];
	int		var_len;

	var_len = 0;
	shell->i++;
	if (input[shell->i] == '\0' || input[shell->i] == ' ')
	{
		buffer[(*buf_index)++] = '$';
		return (shell->i);
	}
	if (input[shell->i] == '$')
	{
		shell->i++;
		if (handle_dollar_case(buffer, buf_index, shell) == -1)
			return (-1);
		return (shell->i);
	}
	if (get_variable_name(input, var_name, &var_len, shell) == -1)
		return (-1);
	copy_variable_value(var_name, buffer, buf_index, shell);
	return (shell->i);
}

char	**tokenize_input(char *input, t_minishell *shell)
{
	char	**tokens;
	int		j;

	shell->i = 0;
	j = 0;
	tokens = ft_calloc(100, sizeof(char *));
	if (!tokens)
		return (NULL);
	while (input[shell->i])
	{
		while (input[shell->i] == ' ')
			shell->i++;
		if (process_token(input, tokens, &j, shell) == -1)
		{
			free_tokens_parse(tokens);
			return (NULL);
		}
	}
	tokens[j] = NULL;
	return (tokens);
}
