/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pablogon <pablogon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/01 20:02:47 by pablogon          #+#    #+#             */
/*   Updated: 2024/11/29 18:02:13 by pablogon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	g_signal = 0;

void	exit_shell(t_minishell *shell)
{
	int	status;

	status = shell->exit_status;
	if (shell->tokens)
		free_tokens(&shell->tokens);
	free_shell(&shell);
	close(shell->original_stdin);
	close(shell->original_stdout);
	exit(status);
}

void	init_minishell(t_minishell *shell, char **env)
{
	ft_memset(shell, 0, sizeof(t_minishell));
	create_env_vars(shell, env);
	shell->is_child = false;
	signal(SIGINT, signal_handler);
	signal(SIGQUIT, SIG_IGN);
}

void	execute_cmd(t_minishell *shell, char *input)
{
	if (is_empty_or_whitespace(input))
	{
		free(input);
		return ;
	}
	if (history_length == 0
		|| ft_strcmp(history_get(history_length)->line, input))
		add_history(input);
	shell->tokens = parse_input(input, shell);
	if (shell->tokens && g_signal == 0)
	{
		execute(shell);
		free_command_list(shell->tokens);
		shell->tokens = NULL;
	}
	free(input);
}

int	main(int argc, char **argv, char **env)
{
	t_minishell	shell;
	char		*input;

	shell.original_stdin = dup(STDIN_FILENO);
	shell.original_stdout = dup(STDOUT_FILENO);
	init_minishell(&shell, env);
	while (argc && argv)
	{
		input = readline("\001\033[1;36m\002Minishell ➜ \001\033[0m\002");
		if (g_signal != 0)
			g_signal = 0;
		if (!input)
		{
			printf("exit\n");
			exit_shell(&shell);
		}
		execute_cmd(&shell, input);
		if (g_signal != 0)
		{
			shell.exit_status = 128 + g_signal;
			g_signal = 0;
		}
	}
	exit_shell(&shell);
}
