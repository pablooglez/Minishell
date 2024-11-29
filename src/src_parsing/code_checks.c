/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   code_checks.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albelope <albelope@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/15 13:16:23 by albelope          #+#    #+#             */
/*   Updated: 2024/11/29 01:12:14 by albelope         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_command(t_cmd *cmd)
{
	t_redir		*redir;
	int			i;

	while (cmd)
	{
		printf("Comando: %s\n", cmd->path);
		i = 0;
		while (cmd->arguments && cmd->arguments[i])
		{
			printf("  Argumento %d: %s\n", i, cmd->arguments[i]);
			i++;
		}
		redir = cmd->redir;
		while (redir)
		{
			printf("  Redirección: ");
			if (redir->type == INFILE)
				printf("entrada ");
			else if (redir->type == OUTFILE)
				printf("salida ");
			else if (redir->type == APPEND)
				printf("append ");
			else if (redir->type == HEREDOC)
				printf("heredoc ");
			printf("a archivo: %s\n", redir->file);
			redir = redir->next;
		}
		cmd = cmd->next;
	}
}
