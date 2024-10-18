/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_variables.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albelope <albelope@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/17 22:02:04 by albelope          #+#    #+#             */
/*   Updated: 2024/10/18 20:38:09 by albelope         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

/*
 * Expande una variable si el token empieza por '$'.
 * - Si el token es '$?', devuelve el estado de salida anterior como cadena.
 * - Si es '$VAR', devuelve el valor de la variable de entorno.
 * - Si no existe la variable, devuelve una cadena vacía.
 */
char *expand_variable(char *token, int last_exit_status)
{
    char	*value;

    if (ft_strncmp(token, "$?", 2) == 0)
        return (ft_itoa(last_exit_status)); 
    else if (token[0] == '$' && ft_strlen(token) > 1)
    {
        value = getenv(token + 1);
        if (value)
            return (ft_strdup(value));
		return (ft_strdup(""));
    }
    return (ft_strdup(token));
}

/*
 * Aplica la expansión de variables a todos los argumentos de un comando.
 * Recorre todos los argumentos y aplica 'expand_variable' a cada uno.
 */
void expand_tokens(t_cmd *cmd, t_minishell *shell)
{
    int			i;
    char		*expanded;

    i = 0;
    while (cmd->arguments && cmd->arguments[i] != NULL)
    {
        expanded = expand_variable(cmd->arguments[i], shell->exit_status);
        free(cmd->arguments[i]);
        cmd->arguments[i] = expanded;
        i++;
    }
}
