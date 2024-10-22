/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_pipes.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pabloglez <pabloglez@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 18:02:33 by pabloglez         #+#    #+#             */
/*   Updated: 2024/10/22 19:59:01 by pabloglez        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_command_path(char *cmd, t_minishell *shell)
{
	char	*path_env;
	char	**paths;
	char	*full_path;
	char	*temp_path;
	int		i;

	path_env = get_env_value(shell->env_vars, "PATH");
	if (!path_env)
		return (NULL);

	paths = ft_split(path_env, ':');
	if (!paths)
		return (NULL);

	i = 0;

	while (paths[i])
	{
		temp_path = ft_strjoin(paths[i], "/");
		if (!temp_path)
		{
			free_array(&paths);
			return (NULL);
		}
		full_path = ft_strjoin(temp_path, cmd);
		free(temp_path);

		if (access(full_path, X_OK) == 0)
		{
			free_array(&paths);
			return (full_path);
		}
		free(full_path);
		i++;
	}
	free_array(&paths);
	return (NULL);
}

void	handle_pipes(t_cmd *cmd, t_minishell *shell)
{
	int		pipe_fds[2];
	pid_t	pid;
	char	*command_path;
	
	if (pipe(pipe_fds) == -1)
	{
		perror("pipe");
		exit(EXIT_FAILURE);
	}

	pid = fork();

	if (pid == -1)
	{
		perror("fork");
		exit(EXIT_FAILURE);
	}

	if (pid == 0)
	{
		close(pipe_fds[0]);
		
		if (dup2(pipe_fds[1], STDOUT_FILENO) == -1)
		{
			perror("dup2");
			exit(EXIT_FAILURE);
		}

		close(pipe_fds[1]);
		
		command_path = get_command_path(cmd->arguments[0], shell);

		if (execve(command_path, cmd->arguments, shell->env) == -1)
		{
			perror("execve");
			exit(EXIT_FAILURE);
		}
		free(command_path);
	}
	else
	{
		close(pipe_fds[1]);
		
		if (dup2(pipe_fds[0], STDIN_FILENO) == -1)
		{
			perror("dup2");
			exit(EXIT_FAILURE);
		}
		close(pipe_fds[0]);

		waitpid(pid, NULL, 0);
	}
}