/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/14 20:00:41 by pabloglez         #+#    #+#             */
/*   Updated: 2024/11/30 19:49:53 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_env(t_minishell *shell)
{
	t_env	*current_env;

	current_env = shell->env_vars;
	while (current_env)
	{
		if (current_env->key && current_env->value && current_env->value[0])
			printf("%s=%s\n", current_env->key, current_env->value);
		current_env = current_env->next;
	}
	shell->exit_status = 0;
}
