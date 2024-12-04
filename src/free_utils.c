/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pabloglez <pabloglez@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/08 16:48:22 by pabloglez         #+#    #+#             */
/*   Updated: 2024/11/29 23:26:16 by pabloglez        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_array(char ***array)
{
	int	i;

	i = -1;
	if (*array)
	{
		while ((*array)[++i])
			free((*array)[i]);
		free(*array);
		*array = NULL;
	}
}

static void	free_redir(t_redir **redir)
{
	t_redir	*next;

	while (*redir)
	{
		next = (*redir)->next;
		if (next)
			next->prev = NULL;
		if ((*redir)->file)
		{
			free((*redir)->file);
			(*redir)->file = NULL;
		}
		if ((*redir)->fd > 0)
			close((*redir)->fd);
		(*redir)->fd = 0;
		free(*redir);
		*redir = NULL;
		*redir = next;
	}
}

static void	free_env_list(t_env **env_var)
{
	t_env	*next;

	while ((*env_var))
	{
		next = (*env_var)->next;
		if (next)
			next->prev = NULL;
		free((*env_var)->key);
		free((*env_var)->value);
		free((*env_var));
		(*env_var) = NULL;
		(*env_var) = next;
	}
}

void	*free_shell(t_minishell **shell)
{
	if ((*shell)->env)
		free_array(&(*shell)->env);
	if ((*shell)->env_vars)
		free_env_list(&(*shell)->env_vars);
	close((*shell)->original_stdin);
	close((*shell)->original_stdout);
	return (NULL);
}

void	*free_tokens(t_cmd **tokens)
{
	t_cmd	*next;

	while (*tokens)
	{
		next = (*tokens)->next;
		if (next)
			next->prev = NULL;
		free_array(&(*tokens)->arguments);
		free_redir(&(*tokens)->redir);
		free(*tokens);
		*tokens = NULL;
		*tokens = next;
	}
	return (NULL);
}
