/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pablogon <pablogon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/01 19:55:18 by pablogon          #+#    #+#             */
/*   Updated: 2024/10/04 18:04:01 by pablogon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void fatal(int code, char *value) 
{
	if (code == MEMORY)
		printf("Error: Memory allocation failed\n");
	if (code == CMD_NOT_FOUND)
		printf("Error: Command not found: %s\n", value);
}

void	ft_error(t_minishell *shell, int code, char * value, int should_exit)
{
	fatal(code, value);

	if (value) 
		free(value);
	if (should_exit)
		exit(shell->exit_status);	//Terminar el programa
}