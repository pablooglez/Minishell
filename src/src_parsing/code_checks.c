/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   code_checks.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pablogon <pablogon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/15 13:16:23 by albelope          #+#    #+#             */
/*   Updated: 2024/11/28 18:25:17 by pablogon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_command(t_cmd *cmd)
{
    int     i;
    t_redir *redir;

    while (cmd)                                                    // Itera sobre cada comando en la lista
    {
        printf("Comando: %s\n", cmd->path);                        // Imprime solo el comando principal

        i = 0;
        while (cmd->arguments && cmd->arguments[i])                // Itera sobre cada argumento del comando
        {
            printf("  Argumento %d: %s\n", i, cmd->arguments[i]);  // Imprime solo los argumentos
            i++;
        }

        redir = cmd->redir;
        while (redir)                                              // Itera sobre cada redirección asociada al comando
        {
            printf("  Redirección: ");                             // Imprime el tipo de redirección
            if (redir->type == INFILE)
                printf("entrada ");                                // Tipo de redirección de entrada (archivo de entrada)
            else if (redir->type == OUTFILE)
                printf("salida ");                                 // Tipo de redirección de salida (archivo de salida)
            else if (redir->type == APPEND)
                printf("append ");                                 // Tipo de redirección de salida en modo append
            else if (redir->type == HEREDOC)
                printf("heredoc ");                                // Tipo de redirección heredoc

            printf("a archivo: %s\n", redir->file);                // Imprime el nombre del archivo para la redirección
            redir = redir->next;                                   // Pasa a la siguiente redirección
        }

        cmd = cmd->next;                                           // Pasa al siguiente comando en la lista
    }
}







