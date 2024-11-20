/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_pipes.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pabloglez <pabloglez@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 18:02:33 by pabloglez         #+#    #+#             */
/*   Updated: 2024/11/19 18:03:33 by pabloglez        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	check_pipes(t_cmd *cmd)
{
	cmd->pipe[0] = -1;
	cmd->pipe[1] = -1;
	if (cmd->type == PIPE)											// Comprueba si el siguiente comando en la lista es un pipe
	{
		if (pipe(cmd->pipe) == -1)												// Crea una tubería. Si falla, imprime un error y sale del programa.
		{
			perror("pipe");
			exit(EXIT_FAILURE);
		}
		cmd->intfd = cmd->pipe[0];
	}
	if (cmd->prev && cmd->prev->type == PIPE)											// Comprueba si el siguiente comando en la lista es un pipe
		cmd->pipe[0] = cmd->prev->intfd;
}