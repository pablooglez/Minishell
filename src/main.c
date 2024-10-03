/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pablogon <pablogon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/01 20:02:47 by pablogon          #+#    #+#             */
/*   Updated: 2024/10/03 21:58:06 by pablogon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void * safe_malloc(t_minishell *shell, size_t size) {
	void * tmp;

	tmp = malloc(size);
	if (!tmp)
		ft_error(shell, MEMORY, NULL);
	return (tmp);
}

void	create_env_vars(t_minishell *shell, char **env)
{
	int	i;
	i = 0;
	
	while (env && env[i] != NULL)
	{
		t_env *node;

		node = safe_malloc(shell, sizeof(t_env));
		i++;
	}
}

void	init_minishell(t_minishell *shell, char **env)
{
	ft_memset(shell, 0, sizeof(t_minishell));
	create_env_vars(shell, env);
	shell->running = 1;
}

int main(int argc, char **argv, char **env) 
{
	t_minishell	shell;
	//char	*input;
	//t_cmd	*command;	
	(void) argc;
	(void) argv;
	
	init_minishell(&shell, env);

	// while (shell.running)
	// {
	// 	input = readline("minishell>");
	// 	if (!input)
	// 	{
	// 		printf("exit\n");
	// 		continue;
	// 	}

	// 	command = parse_input(input);
	// 	if (!command)
	// 	{
	// 		free(input);
	// 		continue;
	// 	}
	// 	if (execute_command(command, &shell) == -1)
	// 		ft_error(&shell, 1, NULL);
	// 	free(input);
	// 	free_command(command);
	// }
	return (0);
}