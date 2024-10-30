/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albelope <albelope@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/15 12:53:22 by albelope          #+#    #+#             */
/*   Updated: 2024/10/30 20:07:46 by albelope         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

t_cmd	*create_new_command(t_minishell *shell)
{
	t_cmd	*new_cmd;

	new_cmd = ft_safe_malloc(shell, sizeof(t_cmd));
	if (!new_cmd)
		return (NULL);
	new_cmd->path = NULL;
	new_cmd->arguments = NULL;
	new_cmd->redir = NULL;
	new_cmd->pipe[0] = -1;
	new_cmd->pipe[1] = -1;
	new_cmd->next = NULL;
	new_cmd->prev = NULL;
	return (new_cmd);
}