/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albelope <albelope@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/25 13:30:05 by albelope          #+#    #+#             */
/*   Updated: 2024/11/29 23:02:45 by albelope         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_arguments(char **args)
{
	int	i;

	i = 0;
	if (!args)
		return ;
	while (args[i])
		free(args[i++]);
	free(args);
}

void	free_redirections(t_redir *redir)
{
	t_redir	*temp;

	while (redir)
	{
		temp = redir;
		redir = redir->next;
		free(temp->file);
		free(temp);
	}
}

void	free_command(t_cmd *cmd)
{
	if (!cmd)
		return ;
	if (cmd->path)
		free(cmd->path);
	if (cmd->arguments)
		free_arguments(cmd->arguments);
	if (cmd->redir)
		free_redirections(cmd->redir);
	free(cmd);
}

void	free_command_list(t_cmd *cmd)
{
	t_cmd	*temp;

	while (cmd)
	{
		temp = cmd->next;
		free_command(cmd);
		cmd = temp;
	}
}

void	free_tokens_parse(char **tokens)
{
	int	i;

	i = 0;
	if (!tokens)
		return ;
	while (tokens[i])
		free(tokens[i++]);
	free(tokens);
}
