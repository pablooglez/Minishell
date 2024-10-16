/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_input.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albelope <albelope@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/11 21:23:53 by albelope          #+#    #+#             */
/*   Updated: 2024/10/16 16:43:02 by albelope         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

/* Inicializa el primer comando y crea la lista de comandos.
Lo que hago aquí es crear la estructura del primer comando con la función
create_new_command` y la devuelvo para empezar a construir la lista de comandos.
Si algo falla al crear el comando, devuelvo NULL para indicar un error. */
t_cmd *initialize_first_command(t_minishell *shell)
{
    t_cmd *cmd;

    cmd = create_new_command(shell);
    if (!cmd)
        return (NULL);
    return (cmd);
}
/* Procesa los argumentos de un comando y los asigna a la estructura t_cmd.
   Esta función toma los tokens que son argumentos 
   (todo lo que no sea un pipe o redirección) y los almacena en la estructura 
   del comando (`t_cmd`). Primero, si aún no hemos asignado el path del comando, 
   lo hacemos aquí. Luego, cuento cuántos argumentos hay hasta el próximo 
   pipe o redirección, y los guardo en `cmd->arguments`. 
   Avanzo en el array de tokens para procesar el resto después de los argumentos. */
int process_arguments(char **tokens, int *i, t_cmd *cmd)
{
    int arg_count;
    int j;

    if (!cmd->path)  // Si el comando principal aún no se ha asignado
        cmd->path = ft_strdup(tokens[*i]);  // Asignar comando principal (path)
    arg_count = 0;
    while (tokens[*i + arg_count]
            && get_redirection_type(tokens[*i + arg_count]) == -1
            && ft_strncmp(tokens[*i + arg_count], "|", 2) != 0)
        arg_count++;  // Contar el número de argumentos hasta encontrar un pipe
    cmd->arguments = ft_calloc(arg_count + 1, sizeof(char *));  // Reservar espacio para los argumentos
    if (!cmd->arguments)
        return (-1);
    j = 0;
    while (j < arg_count)  // Copiar argumentos
    {
        cmd->arguments[j] = ft_strdup(tokens[*i + j]);
        j++;
    }
    cmd->arguments[j] = NULL;  // Terminar la lista de argumentos
    *i += arg_count;  // Avanzar en el array de tokens
    return (0);
}

/* Procesa los tokens y crea la lista de comandos.
   En esta función, recorro todos los tokens que obtuve de la línea de input 
   del usuario. Por cada token, reviso si es un pipe o una redirección.
   Si es un pipe, proceso el pipe creando un nuevo comando. Si es una redirección,
   la proceso y la añado al comando actual. Si es un argumento normal, 
   llamo a `process_arguments` para almacenarlo en el comando. */
int process_tokens(char **tokens, t_cmd *current_cmd, t_minishell *shell)
{
    int i;

    i = 0;
    while (tokens[i])
    {
        if (process_token_pipe(tokens, &i, &current_cmd, shell) == -1)
            return (-1);
        if (process_redirection(tokens, &i, current_cmd) == 0)
            continue;
        if (process_arguments(tokens, &i, current_cmd) == -1)
            return (-1);
    }
    return (0);
}

/* Tokeniza la entrada y estructura los comandos en una lista doblemente enlazada.
   Esta función es el punto de partida. Toma la línea de input que el usuario
   escribe y la divide en tokens con `tokenize_input`. 
   Luego inicializo el primer comando con `initialize_first_command`, y si todo va bien,
   proceso esos tokens para crear una lista de comandos.
   Si algo falla en algún punto, libero la memoria de los tokens y devuelvo NULL.
Al final, devuelvo la lista de comandos que se ha creado. */
t_cmd *parse_input(char *input_line, t_minishell *shell)
{
    char    **tokens;
    t_cmd   *cmd;

    tokens = tokenize_input(input_line);   // Tokenizamos la entrada
    if (!tokens || !tokens[0])
        return (NULL);
    cmd = initialize_first_command(shell);
    if (!cmd)
        return (NULL);
    if (process_tokens(tokens, cmd, shell) == -1)
    {
        free_tokens(tokens);
        return (NULL);
    }
    free_tokens(tokens);
    display_commands(cmd);
    return (cmd); 
}




//completar liberacion en caso de error // freees a saco //