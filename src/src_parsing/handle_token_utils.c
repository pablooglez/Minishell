/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_token_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albelope <albelope@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/29 21:54:12 by albelope          #+#    #+#             */
/*   Updated: 2024/11/29 22:46:17 by albelope         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	process_input_character(char **tokens, char *buffer, int *buf_index,
	t_minishell *shell)
{
	if (shell->e)
	{
		if (shell->e == -1)
			return (-1);
	}
	else if (shell->input[shell->i] == '$')
	{
		shell->i = expand_variable(shell->input, buffer, buf_index, shell);
		if (shell->i == -1)
			return (-1);
	}
	else if (classify_special_token(shell->input[shell->i]) != UNKNOWN)
		return (spec(tokens, buffer, buf_index, shell));
	else
		buffer[(*buf_index)++] = shell->input[shell->i++];
	return (0);
}
