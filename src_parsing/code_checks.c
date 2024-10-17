/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   code_checks.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pabloglez <pabloglez@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/15 13:16:23 by albelope          #+#    #+#             */
/*   Updated: 2024/10/17 18:51:13 by pabloglez        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
            if (current->redir->type == OUTFILE)
                printf(">\n");
            else if (current->redir->type == APPEND)
                printf(">>\n");
            else if (current->redir->type == INFILE)
                printf("<\n");
            else if (current->redir->type == HEREDOC)
                printf("<<\n");
            printf("File: %s\n", current->redir->file);
        }
        current = current->next;
    }
}

