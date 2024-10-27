/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   code_checks.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albelope <albelope@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/15 13:16:23 by albelope          #+#    #+#             */
/*   Updated: 2024/10/26 13:35:35 by albelope         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void display_commands(t_cmd *cmd)
{
    t_cmd *current;
    t_redir *redir;
    int i;

    current = cmd;
    while (current)
    {
        printf("CommanDC: %s\n", current->path);
        i = 0;
        while (current->arguments && current->arguments[i])
        {
            printf("ArgumentDC %d: %s\n", i, current->arguments[i]);
            i++;
        }
        // Recorrer todas las redirecciones
        redir = current->redir;
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
        current = current->next;
    }
}


