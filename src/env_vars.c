/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pablogon <pablogon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/15 18:05:45 by pabloglez         #+#    #+#             */
/*   Updated: 2024/11/27 18:01:59 by pablogon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	*safe_malloc(t_minishell *shell, size_t size)
{
	void	*tmp;

	tmp = malloc(size);
	if (!tmp)
		ft_error(shell, MEMORY_ERROR, NULL, 1);
	return (tmp);
}

void	create_env_vars(t_minishell *shell, char **env)
{
	int	i;

	i = 0;
	while (env && env[i] != NULL)
	{
		t_env *node;
		node = safe_malloc(shell, sizeof(t_env));
		char *equal_sign = ft_strchr(env[i], '=');
		if (equal_sign)
		{
			node->key = ft_strndup(env[i], equal_sign - env[i]);
			node->value = ft_strdup(equal_sign + 1);
		}
		else
		{
			node->key = ft_strdup(env[i]);
			node->value = NULL;
		}
		node->next = shell->env_vars;
		node->prev = NULL;
		if (shell->env_vars)
			shell->env_vars->prev = node;
		shell->env_vars = node;
		i++;
	}
}
