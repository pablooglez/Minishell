/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pablogon <pablogon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/15 12:53:22 by albelope          #+#    #+#             */
/*   Updated: 2024/11/28 19:06:48 by pablogon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_cmd	*create_new_command(t_minishell *shell)
{
	t_cmd	*new_cmd;

	new_cmd = safe_malloc(shell, sizeof(t_cmd));
	if (!new_cmd)
		return (NULL);
	new_cmd->path = NULL;
	new_cmd->arguments = NULL;
	new_cmd->redir = NULL;
	new_cmd->pipe[0] = -1;
	new_cmd->pipe[1] = -1;
	new_cmd->next = NULL;
	new_cmd->prev = NULL;
	new_cmd->intfd = -1;
	new_cmd->outfd = -1;
	new_cmd->type = CMD;
	return (new_cmd);
}

int	initialize_arguments(char **tokens, int *i, t_cmd *cmd)
{
	if (!cmd->path)
	{
		cmd->path = ft_strdup(tokens[*i]);
		if (!cmd->path)
			return (-1);
		(*i)++;
	}
	cmd->arguments = ft_calloc(100, sizeof(char *));
	if (!cmd->arguments)
		return (-1);
	return (0);
}

int	add_argument(char *token, int arg_index, t_cmd *cmd)
{
	cmd->arguments[arg_index] = ft_strdup(token);
	if (!cmd->arguments[arg_index])
		return (-1);
	return (0);
}
