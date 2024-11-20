/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pabloglez <pabloglez@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/01 20:02:47 by pablogon          #+#    #+#             */
/*   Updated: 2024/11/20 19:26:22 by pabloglez        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

volatile sig_atomic_t	g_signal;                                            // Variable global para manejar señales

void	exit_shell(t_minishell *shell)                                       // Función para liberar recursos y salir del shell
{
	int	status;

	status = shell->exit_status;                                            // Guarda el código de salida del shell
	if (shell->tokens)                                                      // Verifica si hay tokens creados
		free_tokens(&shell->tokens);                                        // Libera memoria de los tokens
	free_shell(&shell);                                                     // Libera la estructura del shell
	//rl_clear_history();                                                   // Limpia el historial de readline (desactivado)
    close(shell->original_stdin);
    close(shell->original_stdout);
    exit(status);                                                           // Termina el programa con el estado de salida
}

void	init_minishell(t_minishell *shell, char **env)                      // Inicializa la estructura del shell con el entorno
{
	ft_memset(shell, 0, sizeof(t_minishell));                               // Limpia la memoria de la estructura "shell"
	create_env_vars(shell, env);                                            // Inicializa las variables de entorno
	//shell->running = 1;                                                   // Marca el shell como activo (comentado)
	signal(SIGINT, signal_handler);                                         // Configura el manejador de señal para SIGINT
	signal(SIGQUIT, SIG_IGN);                                               // Ignora SIGQUIT para evitar que el shell termine
}

int main(int argc, char **argv, char **env)
{
    t_minishell shell;                                                      // Estructura del shell
    char *input;                                                            // Variable para almacenar la entrada del usuario

    (void)argc;                                                             // Ignora el argumento de número de argumentos
    (void)argv;                                                             // Ignora el argumento de vector de argumentos
   	shell.original_stdin = dup(STDIN_FILENO);
	shell.original_stdout = dup(STDOUT_FILENO);

    init_minishell(&shell, env);                                            // Inicializa el shell con el entorno
    while (1)                                                               // Bucle principal del shell
    {
        if (g_signal)                                                       // Verifica si se recibió una señal
        {
            g_signal = 0;                                                   // Restablece el indicador de señal
            printf("\n");                                                   // Imprime una nueva línea
        }
        input = readline("\001\033[1;36m\002Minishell ➜ \001\033[0m\002");  // Lee la entrada del usuario
        if (!input)                                                         // Si se recibe EOF (Ctrl+D)
        {
            printf("exit\n");                                               // Imprime "exit" y termina el shell
            exit_shell(&shell);                                             // Llama a la función para liberar recursos y salir
        }
        if (input && input[0] == '\0')                                      // Verifica si la entrada está vacía
        {
            free(input);                                                    // Libera la memoria de la entrada
            continue;                                                       // Continua al siguiente ciclo del bucle
        }
        shell.tokens = parse_input(input, &shell);                          // Analiza la entrada y crea los tokens
        if (shell.tokens)                                                   // Si hay tokens válidos
        {
            execute_command(&shell);                                        // Ejecuta el comando basado en los tokens
            free_command_list(shell.tokens);                                // Libera la lista de comandos
            shell.tokens = NULL;                                            // Establece los tokens a NULL
        }
        free(input);                                                        // Libera la memoria de la entrada
    }
    exit_shell(&shell);                                                     // Retorna 0 al terminar (no debería llegar aquí)
}
