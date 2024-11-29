/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pablogon <pablogon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/29 19:58:04 by pablogon          #+#    #+#             */
/*   Updated: 2024/11/29 20:03:04 by pablogon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*try_command_in_paths(char *cmd, char **paths)
{
	int		i;
	char	*temp_path;
	char	*full_path;

	i = 0;
	while (paths[i])
	{
		temp_path = ft_strjoin(paths[i], "/");
		if (!temp_path)
			return (NULL);
		full_path = ft_strjoin(temp_path, cmd);
		free(temp_path);
		if (full_path && access(full_path, X_OK) == 0)
			return (full_path);
		free(full_path);
		i++;
	}
	return (NULL);
}

static char	**get_paths_from_env(t_minishell *shell)
{
	char	*path_env;

	path_env = get_env_value(shell->env_vars, "PATH");
	if (!path_env)
		return (NULL);
	return (ft_split(path_env, ':'));
}

char	*get_command_path(char *cmd, t_minishell *shell)
{
	char	**paths;
	char	*full_path;

	full_path = check_absolute_or_relative_path(cmd);
	if (full_path)
		return (full_path);
	paths = get_paths_from_env(shell);
	if (!paths)
		return (NULL);
	full_path = try_command_in_paths(cmd, paths);
	free_array(&paths);
	return (full_path);
}
