/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_pipes.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albelope <albelope@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/15 12:43:54 by albelope          #+#    #+#             */
/*   Updated: 2024/10/30 20:09:02 by albelope         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	check_pipe_errors(char **tokens, int *i)
{
	if (*i == 0 || !tokens[*i + 1] || ft_strncmp(tokens[*i + 1], "|", 2) == 0)
	{
		printf("Error: Pipe mal posicionado.\n");
		return (-1);
	}
	(*i)++;
	return (0);
}

int	create_next_command(t_cmd **cmd, t_minishell *shell)
{
	(*cmd)->next = create_new_command(shell);
	if (!(*cmd)->next)
		return (-1);
	(*cmd)->next->prev = *cmd;
	*cmd = (*cmd)->next;
	return (0);
}

int	process_token_pipe(char **tokens, int *i, t_cmd **cmd, t_minishell *shell)
{
	if (ft_strncmp(tokens[*i], "|", 2) == 0)
	{
		if (check_pipe_errors(tokens, i) == -1)
			return (-1);
		if (create_next_command(cmd, shell) == -1)
			return (-1);
		return (0);
	}
	return (1);
}
