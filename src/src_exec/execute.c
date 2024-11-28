/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pablogon <pablogon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/09 16:59:29 by pabloglez         #+#    #+#             */
/*   Updated: 2024/11/28 01:46:29 by pablogon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	count_nodes(t_env *env_vars)
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

char	**free_env_array(char ***env_array, int i)
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

static void	execute_child(t_minishell *shell, t_cmd *cmd)
{
	char	*command_path;

	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	safe_dup2(cmd->pipe[0], STDIN_FILENO);
	safe_dup2(cmd->pipe[1], STDOUT_FILENO);
	safe_close(cmd->intfd);
	if (cmd->arguments && handle_builtin(cmd, shell))
		exit(shell->exit_status);
	handle_redirection(shell, cmd->redir, -1);
	if (!cmd->arguments)
		exit(shell->exit_status);
	command_path = get_command_path(cmd->arguments[0], shell);
	if (!command_path)
		ft_error(shell, CMD_NOT_FOUND, cmd->arguments[0], 1);
	if (execve(command_path, cmd->arguments,
			env_vars_to_array(shell->env_vars)) == -1)
	{
		ft_error(shell, CMD_NOT_FOUND, cmd->arguments[0], 1);
		free(command_path);
	}
}

static void	execute_commands(t_minishell *shell)
{
	t_cmd	*cmd;

	cmd = shell->tokens;
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
	while (cmd)
	{
		if (handle_pipe(cmd) == 1)
			ft_error(shell, MEMORY_ERROR, NULL, 1);
		cmd->pid = fork();
		if (cmd->pid < 0)
			ft_error(shell, MEMORY_ERROR, NULL, 1);
		else if (cmd->pid == 0)
			execute_child(shell, cmd);
		safe_close(cmd->pipe[0]);
		safe_close(cmd->pipe[1]);
		cmd = cmd->next;
	}
}

void	execute(t_minishell *shell)
{
	t_cmd	*cmd;

	cmd = shell->tokens;
	if (cmd->arguments && (!cmd->prev || cmd->prev->type != 3)
		&& cmd->type != 3 && handle_builtin(cmd, shell))
	{
		dup2(shell->original_stdin, STDIN_FILENO);
		dup2(shell->original_stdout, STDOUT_FILENO);
	}
	else
	{
		execute_commands(shell);
		while (cmd)
		{
			if (cmd->pid != -1)
			{
				waitpid(cmd->pid, &shell->exit_status, 0);
				if (WIFEXITED(shell->exit_status))
					shell->exit_status = WEXITSTATUS(shell->exit_status);
				else if (WIFSIGNALED(shell->exit_status))
					shell->exit_status = 128 + WTERMSIG(shell->exit_status);
			}
			cmd = cmd->next;
		}
	}
	delete_heredoc(shell);
	if (shell->exit_status >= 128)
		write(1, "\n", 1);
	signal(SIGINT, signal_handler);
	signal(SIGQUIT, SIG_IGN);
}

