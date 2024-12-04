/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_vars.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pablogon <pablogon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/15 18:05:45 by pabloglez         #+#    #+#             */
/*   Updated: 2024/11/29 20:47:22 by pablogon         ###   ########.fr       */
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

static t_env	*create_env_node(t_minishell *shell, const char *env_entry)
{
	t_env	*node;
	char	*equal_sign;

	node = safe_malloc(shell, sizeof(t_env));
	equal_sign = ft_strchr(env_entry, '=');
	if (equal_sign)
	{
		node->key = ft_strndup(env_entry, equal_sign - env_entry);
		node->value = ft_strdup(equal_sign + 1);
	}
	else
	{
		node->key = ft_strdup(env_entry);
		node->value = NULL;
	}
	node->next = NULL;
	node->prev = NULL;
	return (node);
}

static void	add_env_node(t_minishell *shell, t_env *node)
{
	if (shell->env_vars)
		shell->env_vars->prev = node;
	node->next = shell->env_vars;
	shell->env_vars = node;
}

void	create_env_vars(t_minishell *shell, char **env)
{
	int		i;
	t_env	*node;

	i = 0;
	while (env && env[i])
	{
		node = create_env_node(shell, env[i]);
		add_env_node(shell, node);
		i++;
	}
}
