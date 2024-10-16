/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   code_checks.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albelope <albelope@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/15 13:16:23 by albelope          #+#    #+#             */
/*   Updated: 2024/10/16 14:11:12 by albelope         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void display_commands(t_cmd *cmd)
{
    t_cmd *current;
    int i;

    current = cmd;
    while (current)
    {
        printf("Command: %s\n", current->path);
        i = 0;
        while (current->arguments && current->arguments[i])
        {
            printf("Argument %d: %s\n", i, current->arguments[i]);
            i++;
        }
        if (current->redir)
        {
            printf("Redirection: ");
            if (current->redir->type == REDIR_OUT)
                printf(">\n");
            else if (current->redir->type == REDIR_APPEND)
                printf(">>\n");
            else if (current->redir->type == REDIR_IN)
                printf("<\n");
            else if (current->redir->type == REDIR_HEREDOC)
                printf("<<\n");
            printf("File: %s\n", current->redir->ruta);
        }
        current = current->next;
    }
}

