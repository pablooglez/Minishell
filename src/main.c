/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albelope <albelope@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/01 20:02:47 by pablogon          #+#    #+#             */
/*   Updated: 2024/10/17 20:24:39 by albelope         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

volatile sig_atomic_t	g_signal;

void	exit_shell(t_minishell *shell)							//Función que se encarga de liberar recursos y salir del shell.
{
	int	status;

	status = shell->exit_status;								// Guarda el estado de salida del shell.
	printf("exit\n");											// Muestra el mensaje "exit" en la terminal.
	if(shell->tokens)											// Si existen tokens creados durante la ejecución...
		free_tokens(&shell->tokens);							// ...los libera de memoria.
	free_shell(&shell);											// Libera la estructura del shell.
	exit(status);												// Sale del programa con el código de estado almacenado en "status".
}

void	init_minishell(t_minishell *shell, char **env)			// Inicializa la estructura del shell con el entorno.
{
	ft_memset(shell, 0, sizeof(t_minishell));					// Limpia la memoria de la estructura "shell" para evitar valores basura.
	create_env_vars(shell, env);								// Inicializa las variables de entorno para el shell.
	shell->running = 1;											// Establece el flag "running" en 1, indicando que el shell está activo.
}

int main(int argc, char **argv, char **env)						// Función principal del programa.
{
	t_minishell	shell;											// Define la estructura "shell" donde se almacenarán los datos del shell.
	char	*input;

	(void) argc;												// Evita advertencias por no usar el argumento "argc".
	(void) argv;												// Evita advertencias por no usar el argumento "argv".
	
	init_minishell(&shell, env);								// Llama a la función que inicializa el shell con las variables de entorno.

	while (true)												// Bucle infinito para mantener el shell ejecutándose continuamente.
	{
		input = read_input();	
		printf("(MAIN.c)		Input es:		%s\n", input);								// Llama a la función "read_input" para obtener la entrada del usuario.
		if (!input)												// Si no se recibe ninguna entrada (posible EOF)...
			exit_shell(&shell);									// ...se llama a "exit_shell" para liberar recursos y salir del shell.
		else if (input[0] != '\0')								// Si la entrada no está vacía...
		{ 
			printf("(MAIN.c) 		Input es:		%s\n", input);
			shell.tokens = parse_input(ft_strdup(input), &shell);				// Llama a la función de parseo (implementada por ALBELOPE).
			if (shell.tokens)
			{
				t_cmd *cmd = shell.tokens;
				while (cmd)
				{
					printf("(MAIN.c) 		Procesando comando:	%s\n", cmd->path);
					cmd = cmd->next;
				}
				execute_command(&shell);
			}
			else
			{
				printf("(MAIN.c)ERROR: shell.tokens no esta asignado\n");	
			}								
										// ...llama a la función de ejecución de comandos (implementada por PABLOGON).
		}
		free(input);											// Libera la memoria ocupada por la entrada del usuario después de procesarla.
	}
	return (0);													// Retorna 0 para indicar que el programa terminó correctamente.
}
