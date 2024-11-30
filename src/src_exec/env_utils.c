/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pabloglez <pabloglez@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/29 21:17:39 by pablogon          #+#    #+#             */
/*   Updated: 2024/11/29 23:50:51 by pabloglez        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	count_nodes(t_env *env_vars)
{
	int	count;

	count = 0;
	while (env_vars)
	{
		count++;
		env_vars = env_vars->next;
	}
	return (count);
}

static char	**free_env_array(char ***env_array, int i)
{
	while (i > -1)
		free((*env_array)[i--]);
	free(*env_array);
	return (NULL);
}

char	**env_vars_to_array(t_env *env_vars)
{
	char	**env_array;
	int		len;
	int		i;

	env_array = malloc((count_nodes(env_vars) + 1) * sizeof(char *));
	if (!env_array)
		return (NULL);
	i = 0;
	while (env_vars)
	{
		len = ft_strlen(env_vars->key) + 1;
		if (env_vars->value)
			len += ft_strlen(env_vars->value);
		env_array[i] = malloc((len + 1) * sizeof(char));
		if (!env_array[i])
			return (free_env_array(&env_array, i));
		env_array[i] = ft_strjoin2(env_vars->key, "=", env_vars->value);
		if (!env_array[i])
			return (free_env_array(&env_array, --i));
		env_vars = env_vars->next;
		i++;
	}
	env_array[i] = NULL;
	return (env_array);
}
