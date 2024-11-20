/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pabloglez <pabloglez@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/09 16:59:29 by pabloglez         #+#    #+#             */
/*   Updated: 2024/11/20 20:02:51 by pabloglez        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	safe_dup2(int fd1, int fd2)
{
	if (fd1 < 0 || fd2 < 0)
		return;
	if (dup2(fd1, fd2) == -1)
		exit(1);
	close(fd1);
}

int	execute_command(t_minishell *shell)
{
	t_cmd	*cmd;																			// Inicializa un puntero a la lista de comandos (tokens) en la estructura del shell
	pid_t	pid;																			// Declaración de una variable para almacenar el ID del proceso creado por fork()
	char	*command_path;																	// Declaración de un puntero que contendrá la ruta completa del comando a ejecutar

	cmd = shell->tokens;																	// Asigna el inicio de la lista de comandos a la variable cmd
	
	while (cmd)																				// Comienza un bucle que continúa mientras haya comandos para procesar
	{		
		if ((!cmd->prev || cmd->prev->type != 3) && cmd->type != 3 && handle_builtin(cmd, shell))								// Verifica si el comando es un built-in (comandos internos como echo, cd, etc.)
		{
			dup2(shell->original_stdin, STDIN_FILENO);
			dup2(shell->original_stdout, STDOUT_FILENO);
			return (0);																	// Sale de la función si se ejecuta un comando built-in
		}
		check_pipes(cmd);
		pid = fork();																	// Crea un nuevo proceso hijo con fork()
		if (pid == 0)																	// Verifica si el proceso actual es el hijo (pid == 0 significa proceso hijo)
		{
			safe_dup2(cmd->pipe[0], STDIN_FILENO);
			safe_dup2(cmd->pipe[1], STDOUT_FILENO);
			if (handle_builtin(cmd, shell))
				exit(shell->exit_status);
			handle_redirection(cmd);
																						// Obtiene la ruta completa del comando (ejemplo: "/bin/ls" si el comando es "ls")
			command_path = get_command_path(cmd->arguments[0], shell);					// Llama a get_command_path para buscar el comando en los directorios de PATH
				
			if (!command_path)															// Si el comando no se encuentra, maneja el error
			{
				ft_error(shell, CMD_NOT_FOUND, cmd->arguments[0], 1);					// Imprime un error indicando que el comando no se encuentra
				exit(EXIT_FAILURE);														// Sale del proceso hijo con un código de error
			}

																							// Ejecuta el comando usando execve, que reemplaza el proceso actual con el nuevo programa
			if (execve(command_path, cmd->arguments, shell->env) == -1)					// Si execve falla, imprime un mensaje de error
			{
				ft_error(shell, CMD_NOT_FOUND, cmd->arguments[0], 1);					// Imprime un mensaje de error si execve falla
				exit(EXIT_FAILURE);														// Sale del proceso hijo si execve falla				}
				free(command_path);															// Libera la memoria de command_path si execve se ejecuta correctamente
			}
		}
		else if (pid < 0)																// Si fork() falla, verifica que pid sea menor que 0 (indica un error)
		{
			ft_error(shell, MEMORY_ERROR, NULL, 0);											// Si hubo un error al crear el proceso, maneja el error de memoria
			exit(EXIT_FAILURE);															// Sale de la función con un código de error
		}
		else																			// Si estamos en el proceso padre (pid > 0)
		{
			waitpid(pid, &shell->exit_status, 0);										// Espera a que el proceso hijo termine su ejecución
																						// Comprueba el estado de salida del proceso hijo
			if (WIFEXITED(shell->exit_status))											// Si el proceso terminó normalmente (sin señal)
				shell->exit_status = WEXITSTATUS(shell->exit_status);					// Obtiene el código de salida del proceso hijo
			else if (WIFSIGNALED(shell->exit_status))									// Si el proceso terminó debido a una señal
				shell->exit_status = 128 + WTERMSIG(shell->exit_status);				// Obtiene el número de la señal que lo finalizó
		}
		cmd = cmd->next;																	// Avanza al siguiente comando en la lista de comandos
	}
	cmd = shell->tokens;
	while (cmd)
	{
		if (cmd->type == PIPE)
		{
			if (cmd->pipe[0] != -1)
				close(cmd->pipe[0]);
			if (cmd->pipe[1] != -1)
				close(cmd->pipe[1]);
		}
		cmd = cmd->next;
	}
	return (0);																				// Retorna 0 al finalizar la ejecución de todos los comandos
}

