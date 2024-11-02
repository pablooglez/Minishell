/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_pipes.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albelope <albelope@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 18:02:33 by pabloglez         #+#    #+#             */
/*   Updated: 2024/11/02 16:43:25 by albelope         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void handle_pipes(t_cmd *cmd, t_minishell *shell)
{
	int		pipe_fds[2];												// Array de dos enteros que almacenará los descriptores de archivos para los extremos de la tubería.
	pid_t	pid;														// Variable para almacenar el ID del proceso hijo creado con fork().
	char	*command_path;												// Variable para almacenar la ruta completa del comando que se va a ejecutar.

	printf("(DEBUG) Entrando a handle_pipes para comando: '%s'\n", cmd->arguments[0]);
	if (pipe(pipe_fds) == -1)											// Crea una tubería. Si falla, imprime un error y sale del programa.
	{
		perror("pipe");
		exit(EXIT_FAILURE);
	}
	printf("(DEBUG) Pipe creado correctamente: fd[0]=%d, fd[1]=%d\n", cmd->pipe[0], cmd->pipe[1]);
	pid = fork();														// Crea un nuevo proceso hijo.
	if (pid == -1)														// Si la llamada a fork falla, imprime un error y sale del programa.
	{
		perror("fork");
		exit(EXIT_FAILURE);
	}

	if (pid == 0)														// Código que ejecuta el proceso hijo.
	{
		printf("(DEBUG - HIJO) Proceso hijo iniciado para comando: '%s'\n", cmd->arguments[0]);
		close(pipe_fds[0]);												// Cierra el extremo de lectura de la tubería (solo escribirá en ella).
		
		if (dup2(pipe_fds[1], STDOUT_FILENO) == -1)						// Redirige la salida estándar (STDOUT) hacia la tubería (pipe_fds[1]).
		{
			perror("dup2");
			exit(EXIT_FAILURE);
		}
		printf("(DEBUG - HIJO) Redirección de STDOUT completada para '%s'\n", cmd->arguments[0]);
		close(pipe_fds[1]);												// Cierra el extremo de escritura de la tubería una vez redirigida la salida.

		command_path = get_command_path(cmd->arguments[0], shell);		// Obtiene la ruta completa del comando a ejecutar.

		if (execve(command_path, cmd->arguments, shell->env) == -1)		// Ejecuta el comando usando execve.
		{
			perror("execve");											// Si execve falla, imprime un error y sale del proceso hijo.
			exit(EXIT_FAILURE);
		}
		free(command_path);												// Libera la memoria asignada a la ruta del comando.
	}
	else																// Código que ejecuta el proceso padre.
	{
		printf("(DEBUG - PADRE) Proceso padre esperando al hijo con pid %d\n", pid);
		close(pipe_fds[1]);												// Cierra el extremo de escritura de la tubería (solo leerá de ella).
		
		if (dup2(pipe_fds[0], STDIN_FILENO) == -1)						// Redirige la entrada estándar (STDIN) hacia la tubería (pipe_fds[0]).
		{
			perror("dup2");
			exit(EXIT_FAILURE);
		}
		printf("(DEBUG - PADRE) Redirección de STDIN completada\n");
		close(pipe_fds[0]);												// Cierra el extremo de lectura de la tubería una vez redirigida la entrada.

		waitpid(pid, NULL, 0);											// El proceso padre espera a que termine el proceso hijo.
		printf("(DEBUG - PADRE) Proceso hijo con pid %d terminado\n", pid);
	}
}
