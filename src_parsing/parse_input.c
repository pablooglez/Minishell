/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_input.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albelope <albelope@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/11 21:23:53 by albelope          #+#    #+#             */
/*   Updated: 2024/10/29 21:07:45 by albelope         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

/* Inicializa el primer comando y crea la lista de comandos.
   Lo que hago aquí es crear la estructura del primer comando con la función
   `create_new_command` y la devuelvo para empezar a construir la lista de comandos.
   Si algo falla al crear el comando, devuelvo NULL para indicar un error. */
t_cmd *initialize_first_command(t_minishell *shell)
{
    t_cmd *cmd;

    cmd = create_new_command(shell);                                                           // Crea un nuevo comando usando la función create_new_command
    if (!cmd)                                                                                  // Verifica si la creación del comando falló
        return (NULL);                                                                         // Retorna NULL en caso de error
    //printf("Inicializado primer comando.\n");                                                // Mensaje de depuración
    return (cmd);                                                                              // Devuelve el primer comando creado
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
    int arg_count;                                                                             // Contador para la cantidad de argumentos
    int j;                                                                                     // Índice para copiar los argumentos

    if (!cmd->path)                                                                            // Si el comando principal aún no se ha asignado
        cmd->path = ft_strdup(tokens[*i]);                                                     // Asignar comando principal (path)
    arg_count = 0;
    while (tokens[*i + arg_count]                                                              // Recorre los tokens hasta encontrar un pipe o redirección
            && get_redirection_type(tokens[*i + arg_count]) == -1
            && ft_strncmp(tokens[*i + arg_count], "|", 2) != 0)
        arg_count++;                                                                           // Contar el número de argumentos hasta encontrar un pipe
    cmd->arguments = ft_calloc(arg_count + 1, sizeof(char *));                                 // Reservar espacio para los argumentos
    if (!cmd->arguments)                                                                       // Verifica si hubo error al reservar memoria
        return (-1);                                                                           // Retorna -1 en caso de error
    j = 0;
    while (j < arg_count)                                                                      // Copiar argumentos a la estructura del comando
    {
        cmd->arguments[j] = ft_strdup(tokens[*i + j]);                                         // Duplicar el token y almacenarlo
        //printf("Argumento procesadoPA: '%s'\n", cmd->arguments[j]);                          // Depuración para mostrar el argumento procesado
        j++;
    }
    cmd->arguments[j] = NULL;                                                                  // Terminar la lista de argumentos con NULL
    *i += arg_count;                                                                           // Avanzar en el array de tokens
    return (0);                                                                                // Retorna 0 si todo fue procesado correctamente
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
    while (tokens[i])                                                                          // Recorre todos los tokens hasta el final (NULL)
    {
        //printf("Procesando token[%d]: '%s'\n", i, tokens[i]); // Depuración                    // Muestra el token que se está procesando
        if (process_token_pipe(tokens, &i, &current_cmd, shell) == -1)                         // Procesa pipes y crea nuevos comandos
        {
            //printf("Error al procesar pipe en token[%d]: '%s'\n", i, tokens[i]); // Depuración // Mensaje de error si falla el procesamiento del pipe
            return (-1);                                                                       // Retorna -1 si hubo un error
        }
        if (process_redirection(tokens, &i, current_cmd) == 0)                                 // Procesa redirecciones (>, <, >>, <<)
        {
            //printf("Redirección procesada en token[%d]: '%s'\n", i, tokens[i]); // Depuración  // Depuración para mostrar redirección procesada
            continue;                                                                          // Salta al siguiente token si se procesó una redirección
        }
        if (process_arguments(tokens, &i, current_cmd) == -1)                                 // Procesa los argumentos restantes del comando
        {
            //printf("Error al procesar argumentos en token[%d]: '%s'\n", i, tokens[i]); // Depuración // Mensaje de error si falla el procesamiento de argumentos
            return (-1);                                                                       // Retorna -1 si hubo un error
        }
    }
    return (0);                                                                                // Retorna 0 si todos los tokens se procesaron correctamente
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
    char    **tokens;                                                                          // Array para almacenar tokens de la entrada
    t_cmd   *cmd;                                                                              // Puntero al primer comando de la lista

    if (contains_invalid_characters(input_line))                                               // Verifica si la entrada contiene caracteres no permitidos
        return (NULL);                                                                         // Devuelve NULL y no continúa si hay caracteres inválidos
    tokens = tokenize_input(input_line);                                                       // Tokenizamos la entrada
    if (!tokens || !tokens[0])
    {
        free_tokens_parse(tokens);                                                              // Verifica si no se pudieron generar tokens
        return (NULL);                                                                         // Retorna NULL si hubo un error en la tokenización
    }
    //printf("Lista de tokens generados en `parse_input`:\n");                                 // Mensaje de depuración para mostrar tokens generados
    //for (int k = 0; tokens[k]; k++)                                                             // Recorre y muestra cada token (para depuración)
    //{
        //printf("Token[%d]: '%s'\n", k, tokens[k]);                                           // Mostrar cada token
    //}
    cmd = initialize_first_command(shell);                                                     // Inicializa el primer comando de la lista
    if (!cmd)
    {
        free_tokens_parse(tokens);                                                                                   // Verifica si hubo error al inicializar el comando
        return (NULL);                                                                         // Retorna NULL si no se pudo crear el comando
    }
    if (tokens[0][0] == '$') // Detecta si el primer token es una variable
    {
       // printf("Detectado variable al inicio, utilizando echo como comando\n");
        cmd->path = ft_strdup("echo");  // Usa "echo" como comando predeterminado
    }
    if (process_tokens(tokens, cmd, shell) == -1)                                              // Procesa los tokens para crear la lista de comandos
    {
        //printf("Error al procesar tokens en `parse_input`.\n");                              // Mensaje de error en caso de falla
        free_tokens_parse(tokens);
        free_command(cmd);                                                             // Libera los tokens si hubo un error
        return (NULL);                                                                         // Retorna NULL
    }
    expand_tokens(cmd, shell);                                                                 // Expande variables y reemplazos en los tokens
    free_tokens_parse(tokens);                                                                 // Libera el array de tokens después de usarlos
    display_commands(cmd);                                                                     // Muestra los comandos generados (para depuración)
    return (cmd);                                                                              // Devuelve el primer comando de la lista
}

// completar liberacion en caso de error // freees a saco     