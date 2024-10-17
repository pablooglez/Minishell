/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_pipes.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albelope <albelope@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/15 12:43:54 by albelope          #+#    #+#             */
/*   Updated: 2024/10/16 18:40:11 by albelope         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

/*
** Verifica si el pipe esta bien, si no hay comando antes del pipe (i = 0) o después.
	o dos pipes consecutivos*/
int check_pipe_errors(char **tokens, int *i)
{
    if (*i == 0)
    {
        printf("Erroraso: falta un comando antes del pipe.\n");
        return (-1);
    }
    (*i)++;
    if (!tokens[*i])
    {
        printf("Errorsito: falta un comando después del pipe.\n");
        return (-1);
    }
    if (ft_strncmp(tokens[*i], "|", 2) == 0)
    {
        printf("Errorsito: falta un comando entre los pipes.\n");
        return (-1);
    }
    return (0);
}
/*
** Crea un nuevo comando en la lista doblemente enlazada cuando se encuentra un pipe.
** Actualiza el puntero `cmd` para apuntar al nuevo comando.
** Devuelve -1 si ocurre un error en la creación del comando, 0 si todo está bien.
*/

int create_next_command(t_cmd **cmd, t_minishell *shell)
{
    (*cmd)->next = create_new_command(shell);
    if (!(*cmd)->next)
        return (-1);
    (*cmd)->next->prev = *cmd;
    *cmd = (*cmd)->next;
    return (0);
}
/*
** Procesa un token de pipe ('|') en la cadena de tokens.
** Si encuentra un pipe, verifica si está correctamente colocado y crea un nuevo comando.
** Devuelve:
** -1 en caso de error.
**  0 si encuentra un pipe y todo está bien.
**  1 si el token actual no es un pipe, para que se continúe con otros tokens.
*/
int process_token_pipe(char **tokens, int *i, t_cmd **cmd, t_minishell *shell)
{
    if (ft_strncmp(tokens[*i], "|", 2) == 0)
    {
        if (check_pipe_errors(tokens, i) == -1)
            return (-1);
        if (create_next_command(cmd, shell) == -1)
            return (-1);
        return (0);
    }
    return (1);
}

// QUEDA MANEJAR QUE SE LIBERE LA MEMORIA CORRECTAMENTE Y VER LA MODULARIDAD DEL CODIGO AUN