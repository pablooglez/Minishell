/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pabloglez <pabloglez@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/14 20:00:41 by pabloglez         #+#    #+#             */
/*   Updated: 2024/10/15 20:11:33 by pabloglez        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_env(t_minishell *shell)
{
	t_env *current_env;

	current_env = shell->env_vars;
	while (current_env)
	{
		if (current_env->key && current_env->value)
			printf("%s=%s\n", current_env->key, current_env->value);
		current_env = current_env->next;
	}
}