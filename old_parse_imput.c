
#include "../include/minishell.h"

t_cmd *initialize_first_command(t_minishell *shell)
{
    t_cmd *cmd;

    cmd = create_new_command(shell);                                                           // Crea un nuevo comando usando la función create_new_command
    if (!cmd)                                                                                  // Verifica si la creación del comando falló
        return (NULL);                                                                         // Retorna NULL en caso de error
    //printf("Inicializado primer comando.\n");                                                // Mensaje de depuración
    return (cmd);                                                                              // Devuelve el primer comando creado
}

/*int process_arguments(char **tokens, int *i, t_cmd *cmd)
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
}*/
int	process_arguments(char **tokens, int *i, t_cmd *cmd)
{
	int	arg_count;
	int	j;

	if (!cmd->path)
	{
		cmd->path = ft_strdup(tokens[*i]);
		(*i)++;
	}
	arg_count = 0;
	while (tokens[*i + arg_count]
		&& get_redirection_type(tokens[*i + arg_count]) == NOT_REDIR
		&& ft_strncmp(tokens[*i + arg_count], "|", 2) != 0)
		arg_count++;
	cmd->arguments = ft_calloc(arg_count + 1, sizeof(char *));
	if (!cmd->arguments)
		return (-1);
	j = 0;
	while (j < arg_count)
	{
		cmd->arguments[j] = ft_strdup(tokens[*i + j]);
		j++;
	}
	cmd->arguments[j] = NULL;
	*i += arg_count;
	return (0);
}


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
        if (process_redirection(tokens, &i, current_cmd, shell) == -1)                                 // Procesa redirecciones (>, <, >>, <<)
        {
            //printf("Redirección procesada en token[%d]: '%s'\n", i, tokens[i]); // Depuración  // Depuración para mostrar redirección procesada
            return (-1);                                                                          // Salta al siguiente token si se procesó una redirección
        }
        if (process_arguments(tokens, &i, current_cmd) == -1)                                 // Procesa los argumentos restantes del comando
        {
            //printf("Error al procesar argumentos en token[%d]: '%s'\n", i, tokens[i]); // Depuración // Mensaje de error si falla el procesamiento de argumentos
            return (-1);                                                                       // Retorna -1 si hubo un error
        }
    }
    return (0);                                                                                // Retorna 0 si todos los tokens se procesaron correctamente
}
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
    //display_commands(cmd);
    print_command(cmd);                                                                     // Muestra los comandos generados (para depuración)
    return (cmd);                                                                              // Devuelve el primer comando de la lista
}
   