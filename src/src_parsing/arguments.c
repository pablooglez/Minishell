/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   arguments.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albelope <albelope@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/29 13:00:52 by albelope          #+#    #+#             */
/*   Updated: 2024/11/29 21:09:37 by albelope         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

int	handle_argument(char **tokens, int *i, int *arg_index, t_cmd *cmd)
{
	if (add_argument(tokens[*i], *arg_index, cmd) == -1)
		return (-1);
	(*arg_index)++;
	(*i)++;
	return (0);
}

int	process_arguments(char **tokens, int *i,
			t_cmd *cmd, t_minishell *shell)
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
		else if (handle_argument(tokens, i, &arg_index, cmd) == -1)
			return (-1);
	}
	cmd->arguments[arg_index] = NULL;
	return (0);
}
