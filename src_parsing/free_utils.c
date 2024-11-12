/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albelope <albelope@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/25 13:30:05 by albelope          #+#    #+#             */
/*   Updated: 2024/11/11 19:29:59 by albelope         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void free_arguments(char **args)
{
    int i;
    
    i = 0;
    if (args)                                       // Verifica si args no es NULL
    {
        while (args[i])                             // Recorre cada argumento en args
        {
            free(args[i]);                          // Libera la memoria de cada argumento
            i++;
        }
        free(args);                                 // Libera el arreglo de punteros
    }
}

void free_redirections_prueba(t_redir *redir)
{
    t_redir *temp;

    while (redir)                                   // Recorre cada nodo en la lista de redirecciones
    {
        temp = redir;                               // Guarda el nodo actual
        redir = redir->next;                        // Avanza al siguiente nodo
        free(temp->file);                           // Libera la memoria del archivo de redirección
        free(temp);                                 // Libera el nodo actual
    }
}

void free_command(t_cmd *cmd)
{
    if (cmd)                                        // Verifica si cmd no es NULL
    {
        if (cmd->path)
            free(cmd->path);                        // Libera la memoria de path si existe
        if (cmd->arguments)
            free_arguments(cmd->arguments);         // Libera la lista de argumentos
        if (cmd->redir)
            free_redirections_prueba(cmd->redir);   // Libera la lista de redirecciones
        free(cmd);                                  // Libera el nodo cmd
    }
}

void free_command_list(t_cmd *cmd)
{
    t_cmd *temp;

    while (cmd)                                     // Recorre cada nodo en la lista de comandos
    {
        temp = cmd->next;                           // Guarda el puntero al siguiente nodo
        free_command(cmd);                          // Usa la función para liberar un solo comando
        cmd = temp;                                 // Avanza al siguiente nodo
    }
}

void	free_tokens_parse(char **tokens)
{
	int	i;

	i = 0;
	if (!tokens)
		return ;
	while (tokens[i])
	{
		free(tokens[i]);
		i++;
	}
	free(tokens);
}
void free_redirections(t_redir *redir)
{
	t_redir *temp;																											// Puntero temporal para liberar cada redirección

	while (redir)																											// Recorre toda la lista de redirecciones
	{
		temp = redir;																										// Guarda la redirección actual en `temp`
		redir = redir->next;																								// Avanza a la siguiente redirección
		free(temp->file);																									// Libera la memoria del nombre del archivo de la redirección
		free(temp);																											// Libera la estructura de la redirección
	}
}




