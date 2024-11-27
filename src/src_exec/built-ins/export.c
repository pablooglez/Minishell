/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pablogon <pablogon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/14 20:00:11 by pabloglez         #+#    #+#             */
/*   Updated: 2024/11/27 17:54:48 by pablogon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_env_vars(t_env *env_list)
{
	t_env	*current_env;

	current_env = env_list;
	while (current_env)
	{
		if (current_env->key)
		{
			if (current_env->value)
				printf("declare -x %s=\"%s\"\n",
					current_env->key, current_env->value);
			else
				printf ("declare -x %s\n", current_env->key);
		}
		current_env = current_env->next;
	}
}

int	ft_export(t_minishell *shell, char **arg)
{
	int	i;

	i = 1;
	if (!arg[1])
	{
		print_env_vars(shell->env_vars);
		return (0);
	}
	while (arg && arg[i])
	{
		char *equal_sign = ft_strchr(arg[i], '=');
		if (equal_sign)
		{
			char *key = ft_strndup(arg[i], equal_sign - arg[i]);
			char *value = ft_strdup(equal_sign + 1);
			if (!is_valid_identifier(key))
			{
				free(key);
				free(value);
				return (ft_error(shell, MSG,
						"export: not a valid identifier\n", 0));
			}
			update_env_var(&(shell->env_vars), key, value);
			free(key);
			free(value);
		}
		else
		{
			if (is_valid_identifier(arg[i]))
				update_env_var(&(shell->env_vars), arg[i], "");
			else
				return (ft_error(shell, MSG,
						"export: not a valid identifier\n", 0));
		}
		i++;
	}
	return (0);
}
