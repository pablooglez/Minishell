/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albelope <albelope@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/01 20:02:47 by pablogon          #+#    #+#             */
/*   Updated: 2024/11/16 20:47:33 by albelope         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

volatile sig_atomic_t	g_signal;											//Variable para manejar las señales

void	exit_shell(t_minishell *shell)										//Función que se encarga de liberar recursos y salir del shell.
{
	int	status;

	status = shell->exit_status;											// Guarda el estado de salida del shell.
	if(shell->tokens)														// Si existen tokens creados durante la ejecución...
		free_tokens(&shell->tokens);										// ...los libera de memoria.
	free_shell(&shell);	
	//rl_clear_history();													
	exit(status);															// Sale del programa con el código de estado almacenado en "status".
}

void	init_minishell(t_minishell *shell, char **env)						// Inicializa la estructura del shell con el entorno.
{
	ft_memset(shell, 0, sizeof(t_minishell));								// Limpia la memoria de la estructura "shell" para evitar valores basura.
	create_env_vars(shell, env);											// Inicializa las variables de entorno para el shell.
	shell->running = 1;														// Establece el flag "running" en 1, indicando que el shell está activo.
	signal(SIGINT, signal_handler);											// Configura el manejador de señal para SIGINT
	signal(SIGQUIT, SIG_IGN);												//Ignora SIGQUIT para que no termine el shell
}

int main(int argc, char **argv, char **env)
{
    t_minishell shell;
    char *input;

    (void)argc;
    (void)argv;
    init_minishell(&shell, env);
    while (1)
    {
        if (g_signal)
            g_signal = 0;

        input = read_input();
		if (!input)
        {
            //printf("Salida detectada (EOF).\n");
            exit_shell(&shell);
        }
        else if (input[0] == '\0')
        {
            free(input);
            continue;
        }
        else
        {
            shell.tokens = parse_input(input, &shell);
            if (shell.tokens)
            {
                execute_command(&shell);
                free_command_list(shell.tokens);
                shell.tokens = NULL;
            }
            else
            {
                //printf("(MAIN.c)ERROR: shell.tokens no esta asignado\n");
            }
        }
        free(input);
    }
    return (0);
}

