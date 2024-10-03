/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pablogon <pablogon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/01 20:02:47 by pablogon          #+#    #+#             */
/*   Updated: 2024/10/03 20:56:48 by pablogon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


void	init_minishell(t_minishell *shell, char **env)
{
	ft_memset(shell, 0, sizeof(t_minishell));
	shell->env_vars = init_env_vars(env);
	shell->running = 1;
}
int main(int argc, char **argv, char **env) 
{
	t_minishell	shell;
	char	*input;
	t_cmd	*command;
	int		i;
	
	init_minishell(&shell, env);

	while (shell.running)
	{
		input = readline(PROMPT);
		if (!input)
		{
			printf("exit\n");
			continue; ;
		}

		command = parse_input(input);
		if (!command)
		{
			free(input);
			continue;
		}
		if (execute_command(command, &shell) == -1)
			ft_error("Error: Command not found\n", &shell, 1);
		free(input);
		free_command(command);
	}
	return (0);
}