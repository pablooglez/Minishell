/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albelope <albelope@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/25 13:30:05 by albelope          #+#    #+#             */
/*   Updated: 2024/11/29 22:52:22 by albelope         ###   ########.fr       */
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

void	print_error(const char *msg)
{
	if (msg)
		write(2, msg, ft_strlen(msg));
}

int	print_error_and_return(const char *msg)
{
	if (msg)
		write(2, msg, ft_strlen(msg));
	return (-1);
}

void	print_error_and_exit(const char *msg, int exit_code)
{
	if (msg)
		write(2, msg, ft_strlen(msg));
	exit(exit_code);
}

int	error_handler(const char *msg, int exit_code)
{
	if (msg)
		write(2, msg, ft_strlen(msg));
	if (exit_code >= 0)
		exit(exit_code);
	return (exit_code);
}
