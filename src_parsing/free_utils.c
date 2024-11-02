/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albelope <albelope@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/25 13:30:05 by albelope          #+#    #+#             */
/*   Updated: 2024/11/01 21:34:58 by albelope         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"


void free_arguments(char **args)
{
    int i;
    
    i = 0;
    if (args)
    {
        while (args[i])
        {
            free(args[i]);
            i++;
        }
        free(args);
    }
}

void free_redirections_prueba(t_redir *redir)
{
    t_redir *temp;

    while (redir)
    {
        temp = redir;
        redir = redir->next;
        free(temp->file);
        free(temp);
    }
}

void free_command(t_cmd *cmd)
{
    if (cmd)
    {
        if (cmd->path)
            free(cmd->path);
        if (cmd->arguments)
            free_arguments(cmd->arguments);
        if (cmd->redir)
            free_redirections_prueba(cmd->redir);
        free(cmd);
    }
}


void free_command_list(t_cmd *cmd)
{
    t_cmd *temp;

    while (cmd)
    {
        temp = cmd->next;  // Guarda el puntero al siguiente nodo
        free_command(cmd); // Usa tu función existente para liberar un solo comando
        cmd = temp;        // Avanza al siguiente nodo
    }
}




