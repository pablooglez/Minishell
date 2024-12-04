/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pablogon <pablogon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 17:17:32 by pabloglez         #+#    #+#             */
/*   Updated: 2024/11/30 20:49:19 by pablogon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*check_absolute_or_relative_path(char *cmd)
{
	if (cmd[0] == '/' || (cmd[0] == '.' && cmd[1] == '/')
		|| (cmd[0] == '.' && cmd[1] == '.' && cmd[2] == '/'))
	{
		if (access(cmd, X_OK) == 0)
			return (ft_strdup(cmd));
		else
			return (NULL);
	}
	return (NULL);
}

char	*get_env_value(t_env *env_list, char *key)
{
	t_env	*current_env;

	current_env = env_list;
	while (current_env)
	{
		if (ft_strncmp(current_env->key, key, ft_strlen(key) + 1) == 0)
			return (current_env->value);
		current_env = current_env->next;
	}
	return (NULL);
}

void	update_env_var(t_env **env_list, char *key, char *value)
{
	t_env	*current_env;
	t_env	*new_env_var;

	new_env_var = (t_env *)malloc(sizeof(t_env));
	current_env = *env_list;
	while (current_env)
	{
		if (ft_strncmp(current_env->key, key, ft_strlen(key) + 1) == 0)
		{
			free(new_env_var);
			free(current_env->value);
			current_env->value = ft_strdup(value);
			return ;
		}
		current_env = current_env->next;
	}
	new_env_var->key = ft_strdup(key);
	new_env_var->value = ft_strdup(value);
	new_env_var->next = *env_list;
	new_env_var->prev = NULL;
	if (*env_list)
		(*env_list)->prev = new_env_var;
	*env_list = new_env_var;
}

int	is_valid_identifier(const char *str)
{
	int	i;

	i = 1;
	if (!str || (!ft_isalpha(str[0]) && str[0] != '_'))
		return (0);
	while (str[i])
	{
		if (!ft_isalnum(str[i]) && str[i] != '_')
			return (0);
		i++;
	}
	return (1);
}
