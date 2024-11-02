/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albelope <albelope@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/09 16:59:29 by pabloglez         #+#    #+#             */
/*   Updated: 2024/11/02 16:27:01 by albelope         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	execute_command(t_minishell *shell)
{
	t_cmd	*cmd;																			// Inicializa un puntero a la lista de comandos (tokens) en la estructura del shell
	pid_t	pid;																			// Declaración de una variable para almacenar el ID del proceso creado por fork()
	char	*command_path;																	// Declaración de un puntero que contendrá la ruta completa del comando a ejecutar

	cmd = shell->tokens;																	// Asigna el inicio de la lista de comandos a la variable cmd
	
	while (cmd)																				// Comienza un bucle que continúa mientras haya comandos para procesar
	{ 
		printf("(DEBUG) Procesando comando: '%s'\n", cmd->arguments[0]);
		if (handle_builtin(cmd, shell))														// Verifica si el comando es un built-in (comandos internos como echo, cd, etc.)
		{ 									
			return (0);																		// Sale de la función si se ejecuta un comando built-in
		}
		if (cmd->next && cmd->next->type == PIPE)											// Comprueba si el siguiente comando en la lista es un pipe
		{ 
			printf("(DEBUG) Pipe detectado después del comando: '%s'\n", cmd->arguments[0]);
			handle_pipes(cmd, shell);														// Si es un pipe, llama a la función handle_pipes para manejarlo
			printf("(DEBUG) Comando siguiente después del pipe: '%s'\n", cmd->next ? cmd->next->arguments[0] : "NULL");
		}
		else																				// Si no hay un pipe después del comando actual
		{
			pid = fork();																	// Crea un nuevo proceso hijo con fork()	
			if (pid == 0)																	// Verifica si el proceso actual es el hijo (pid == 0 significa proceso hijo)
			{
				handle_redirection(cmd);													// Maneja las redirecciones de entrada y salida si las hay																					// Obtiene la ruta completa del comando (ejemplo: "/bin/ls" si el comando es "ls")
				command_path = get_command_path(cmd->arguments[0], shell);					// Llama a get_command_path para buscar el comando en los directorios de PATH
				if (!command_path)															// Si el comando no se encuentra, maneja el error
				{
					ft_error(shell, CMD_NOT_FOUND, cmd->arguments[0], 1);					// Imprime un error indicando que el comando no se encuentra
					exit(EXIT_FAILURE);														// Sale del proceso hijo con un código de error
				}																// Ejecuta el comando usando execve, que reemplaza el proceso actual con el nuevo programa
				if (execve(command_path, cmd->arguments, shell->env) == -1)					// Si execve falla, imprime un mensaje de error
				{
					ft_error(shell, CMD_NOT_FOUND, cmd->arguments[0], 1);					// Imprime un mensaje de error si execve falla
					exit(EXIT_FAILURE);														// Sale del proceso hijo si execve falla
				}
				free(command_path);															// Libera la memoria de command_path si execve se ejecuta correctamente
			}
			else if (pid < 0)																// Si fork() falla, verifica que pid sea menor que 0 (indica un error)
				ft_error(shell, MEMORY, NULL, 0);											// Si hubo un error al crear el proceso, maneja el error de memoria
			else																			// Si estamos en el proceso padre (pid > 0)
			{
				waitpid(pid, &shell->exit_status, 0);										// Espera a que el proceso hijo termine su ejecución																			// Comprueba el estado de salida del proceso hijo
				if (WIFEXITED(shell->exit_status))											// Si el proceso terminó normalmente (sin señal)
					shell->exit_status = WEXITSTATUS(shell->exit_status);					// Obtiene el código de salida del proceso hijo
				else if (WIFSIGNALED(shell->exit_status))									// Si el proceso terminó debido a una señal
					shell->exit_status = 128 + WTERMSIG(shell->exit_status);				// Obtiene el número de la señal que lo finalizó
			}
		}
		cmd = cmd->next;																	// Avanza al siguiente comando en la lista de comandos
	}
	return (0);																				// Retorna 0 al finalizar la ejecución de todos los comandos
}

/*int	execute_command(t_minishell *shell)
{
	t_cmd	*cmd;
	pid_t	pid;
	char	*command_path;

	cmd = shell->tokens;
	while (cmd)
	{
		if (handle_builtin(cmd, shell))
		{
			printf("(DEBUG) Ejecutado built-in: %s\n", cmd->arguments[0]);
			return (0);
		}

		if (cmd->next && cmd->next->type == PIPE)
		{
			printf("(DEBUG) Detectado pipe después de comando: %s\n", cmd->arguments[0]);
			handle_pipes(cmd, shell);
		}
		else
		{
			pid = fork();
			if (pid == 0)
			{
				handle_redirection(cmd); // Manejo de redirecciones
				printf("(DEBUG) Redirección manejada para comando: %s\n", cmd->arguments[0]);

				// Obtener la ruta completa del comando
				command_path = get_command_path(cmd->arguments[0], shell);
				printf("(DEBUG) Ruta del comando obtenida: %s\n", command_path ? command_path : "NULL");

				if (!command_path)
				{
					ft_error(shell, CMD_NOT_FOUND, cmd->arguments[0], 1);
					exit(EXIT_FAILURE);
				}

				// Ejecutar el comando
				if (execve(command_path, cmd->arguments, shell->env) == -1)
				{
					ft_error(shell, CMD_NOT_FOUND, cmd->arguments[0], 1);
					printf("(DEBUG) Error en execve para comando: %s\n", cmd->arguments[0]);
					exit(EXIT_FAILURE);
				}
				free(command_path);
			}
			else if (pid < 0)
			{
				ft_error(shell, MEMORY, NULL, 0);
			}
			else
			{
				waitpid(pid, &shell->exit_status, 0);
				if (WIFEXITED(shell->exit_status))
					shell->exit_status = WEXITSTATUS(shell->exit_status);
				else if (WIFSIGNALED(shell->exit_status))
					shell->exit_status = 128 + WTERMSIG(shell->exit_status);
			}
		}
		cmd = cmd->next;
	}
	return (0);
}*/

