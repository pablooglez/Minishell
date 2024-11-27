/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pablogon <pablogon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/01 20:02:47 by pablogon          #+#    #+#             */
/*   Updated: 2024/11/27 17:02:45 by pablogon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

volatile sig_atomic_t	g_signal = 0;

void	exit_shell(t_minishell *shell)
{
	int	status;

	status = shell->exit_status;
	if (shell->tokens)
		free_tokens(&shell->tokens);
	free_shell(&shell);
	//rl_clear_history();
	close(shell->original_stdin);
	close(shell->original_stdout);
	exit(status);
}

void	init_minishell(t_minishell *shell, char **env)
{
	ft_memset(shell, 0, sizeof(t_minishell));
	create_env_vars(shell, env);
	//shell->running = 1;
	signal(SIGINT, signal_handler);
	signal(SIGQUIT, SIG_IGN);
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
		if (g_signal)
		{
			g_signal = 0;
			printf("\n");
		}
		input = readline("\001\033[1;36m\002Minishell ➜ \001\033[0m\002");
		if (!input)
		{
			printf("exit\n");
			exit_shell(&shell);
		}
		if (is_empty_or_whitespace(input))
		{
			free(input);
			continue ;
		}
		add_history(input);
		shell.tokens = parse_input(input, &shell);
		if (shell.tokens)
		{
			execute(&shell);
			free_command_list(shell.tokens);
			shell.tokens = NULL;
		}
		free(input);
	}
	exit_shell(&shell);
}
