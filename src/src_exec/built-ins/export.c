/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pablogon <pablogon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/14 20:00:11 by pabloglez         #+#    #+#             */
/*   Updated: 2024/11/30 20:37:33 by pablogon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	print_env_vars(t_env *env_list)
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

static int	handle_invalid_identifier(t_minishell *shell,
		char *key, char *value)
{
	free(key);
	free(value);
	ft_error(shell, MSG, "export: not a valid identifier\n", 0);
	shell->exit_status = 1;
	return (0);
}

static int	update_env_var_from_arg(t_minishell *shell, char *arg)
{
	char	*equal_sign;
	char	*key;
	char	*value;

	equal_sign = ft_strchr(arg, '=');
	if (equal_sign)
	{
		key = ft_strndup(arg, equal_sign - arg);
		value = ft_strdup(equal_sign + 1);
		if (!is_valid_identifier(key))
			return (handle_invalid_identifier(shell, key, value));
		update_env_var(&(shell->env_vars), key, value);
		free(key);
		free(value);
	}
	else
	{
		if (is_valid_identifier(arg))
			update_env_var(&(shell->env_vars), arg, "");
		else
			shell->exit_status = ft_error(shell, MSG,
					"export: not a valid identifier\n", 0);
	}
	return (0);
}

int	ft_export(t_minishell *shell, char **arg)
{
	int	i;

	i = 1;
	shell->exit_status = 0;
	if (!arg[1])
	{
		print_env_vars(shell->env_vars);
		return (0);
	}
	while (arg && arg[i])
	{
		update_env_var_from_arg(shell, arg[i]);
		i++;
	}
	return (0);
}
