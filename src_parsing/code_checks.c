/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   code_checks.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albelope <albelope@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/15 13:16:23 by albelope          #+#    #+#             */
/*   Updated: 2024/10/29 20:29:52 by albelope         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"


// Función auxiliar para mostrar redirecciones
void display_redirections(t_redir *redir)
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

// Función auxiliar para mostrar argumentos
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
    int is_expanded_command = (current && current->path && current->path[0] == '$');

    while (current)
    {
        if (is_expanded_command)
            printf("Expanded CommanDC: %s\n", current->path);
        else
            printf("CommanDC: %s\n", current->path);

        display_arguments(current->arguments);
        display_redirections(current->redir);
        current = current->next;
    }
}



