/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pablogon <pablogon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/25 13:30:05 by albelope          #+#    #+#             */
/*   Updated: 2024/11/28 16:22:42 by pablogon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* Libera la lista de argumentos */
void free_arguments(char **args)
{
    int i = 0;

    if (!args)
        return;
    while (args[i])
        free(args[i++]);
    free(args);
}

/* Libera la lista de redirecciones */
void free_redirections(t_redir *redir)
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

/* Libera la estructura de comando */
void free_command(t_cmd *cmd)
{
    if (!cmd)
        return;
    if (cmd->path)
        free(cmd->path);
    if (cmd->arguments)
        free_arguments(cmd->arguments);
    if (cmd->redir)
        free_redirections(cmd->redir);
    free(cmd);
}

/* Libera toda la lista de comandos */
void free_command_list(t_cmd *cmd)
{
    t_cmd *temp;

    while (cmd)
    {
        temp = cmd->next;
        free_command(cmd);
        cmd = temp;
    }
}

/* Libera el array de tokens */
void free_tokens_parse(char **tokens)
{
    int i = 0;

    if (!tokens)
        return;
    while (tokens[i])
        free(tokens[i++]);
    free(tokens);
}

/* Imprime el mensaje de error */
void print_error(const char *msg)
{
    if (msg)
        write(2, msg, ft_strlen(msg));
}

/* Imprime el mensaje de error y retorna -1 */
int print_error_and_return(const char *msg)
{
    if (msg)
        write(2, msg, ft_strlen(msg));
    return (-1);
}

/* Imprime el mensaje de error y termina el programa */
void print_error_and_exit(const char *msg, int exit_code)
{
    if (msg)
        write(2, msg, ft_strlen(msg));
    exit(exit_code);
}

int error_handler(const char *msg, int exit_code)
{
    if (msg)
        write(2, msg, ft_strlen(msg));
    
    if (exit_code >= 0)
        exit(exit_code);

    return (exit_code);
}

