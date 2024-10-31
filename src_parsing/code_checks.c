/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   code_checks.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albelope <albelope@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/15 13:16:23 by albelope          #+#    #+#             */
/*   Updated: 2024/10/30 22:20:42 by albelope         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

/*void display_redirections(t_redir *redir)
{
    while (redir)
    {
        printf("Redirection: ");
        if (redir->type == OUTFILE)
            printf(">\n");
        else if (redir->type == APPEND)
            printf(">>\n");
        else if (redir->type == INFILE)
            printf("<\n");
        else if (redir->type == HEREDOC)
            printf("<<\n");
        printf("File: %s\n", redir->file);
        redir = redir->next;
    }
}
void display_arguments(char **arguments)
{
    int i = 0;
    while (arguments && arguments[i])
    {
        printf("ArgumentDC %d: %s\n", i, arguments[i]);
        i++;
    }
}

void display_commands(t_cmd *cmd)
{
    t_cmd *current = cmd;
    //int is_expanded_command = (current && current->path && current->path[0] == '$');

    while (current)
    {
        //if (is_expanded_command)
            //printf("Expanded CommanDC: %s\n", current->path);
       // else
        printf("CommanDC: %s\n", current->path);

        display_arguments(current->arguments);
        display_redirections(current->redir);
        current = current->next;
    }
}*/

void	print_command(t_cmd *cmd)
{
	int		i;
	t_redir	*redir;

	while (cmd)
	{
		printf("Comando: %s\n", cmd->path); // Imprime solo el comando principal
		i = 0;
		while (cmd->arguments && cmd->arguments[i])
		{
			printf("  Argumento %d: %s\n", i, cmd->arguments[i]); // Imprime solo los argumentos
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






