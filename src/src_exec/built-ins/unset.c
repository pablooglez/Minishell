/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/14 20:00:48 by pabloglez         #+#    #+#             */
/*   Updated: 2024/11/30 19:54:44 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	delete_env_var(t_env **env_list, const char *key)
{
	t_env	*current_env;

	current_env = *env_list;
	while (current_env)
	{
		if (ft_strncmp(current_env->key, key, ft_strlen(key) + 1) == 0)
		{
			if (current_env->prev)
				current_env->prev->next = current_env->next;
			if (current_env->next)
				current_env->next->prev = current_env->prev;
			if (current_env == *env_list)
				*env_list = current_env->next;
			free(current_env->key);
			free(current_env->value);
			free(current_env);
			return ;
		}
		current_env = current_env->next;
	}
}

void	ft_unset(t_minishell *shell, char **arg)
{
	int	i;

	i = 1;
	if (!arg[1])
		return ;
	while (arg && arg[i])
	{
		if (is_valid_identifier(arg[i]))
			delete_env_var(&(shell->env_vars), arg[i]);
		else
			write(2, "unset: not a valid identifier\n", 30);
		i++;
	}
	shell->exit_status = 0;
}
