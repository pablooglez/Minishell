/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pabloglez <pabloglez@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/09 16:59:29 by pabloglez         #+#    #+#             */
/*   Updated: 2024/11/29 23:22:20 by pabloglez        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	execute_child(t_minishell *shell, t_cmd *cmd)
{
	char	*command_path;

	shell->is_child = true;
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	safe_dup2(cmd->pipe[0], STDIN_FILENO);
	safe_dup2(cmd->pipe[1], STDOUT_FILENO);
	safe_close(cmd->intfd);
	if (cmd->arguments && handle_builtin(cmd, shell))
		exit(shell->exit_status);
	if (handle_redirection(shell, cmd->redir, -1))
		exit(shell->exit_status);
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

static void	wait_for_processes(t_minishell *shell)
{
	t_cmd	*cmd;

	cmd = shell->tokens;
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
		wait_for_processes(shell);
	}
	delete_heredoc(shell);
	if (shell->exit_status >= 128)
		write(1, "\n", 1);
	signal(SIGINT, signal_handler);
	signal(SIGQUIT, SIG_IGN);
}
