/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_pipes.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pabloglez <pabloglez@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 18:02:33 by pabloglez         #+#    #+#             */
/*   Updated: 2024/10/23 18:06:38 by pabloglez        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char *get_command_path(char *cmd, t_minishell *shell)
{
	char	*path_env;													// Variable para almacenar el valor de la variable de entorno PATH.
	char	**paths;													// Array de cadenas que contendrá las diferentes rutas en PATH.
	char	*full_path;													// Ruta completa del comando después de añadir el nombre del comando.
	char	*temp_path;													// Variable temporal para formar las rutas durante la iteración.
	int		i;															// Contador para iterar sobre las rutas.

	path_env = get_env_value(shell->env_vars, "PATH");					// Busca la variable de entorno "PATH" dentro de las variables de entorno del shell.
	if (!path_env)														// Si no se encuentra la variable PATH, devuelve NULL, ya que no hay rutas disponibles para buscar el comando.
		return (NULL);

	paths = ft_split(path_env, ':');									// Divide la cadena PATH en un array de rutas usando ':' como delimitador.
	if (!paths)															// Si la división falla o no hay rutas, devuelve NULL.
		return (NULL);

	i = 0;																// Inicializa el índice para recorrer las rutas en el array paths.

	while (paths[i])													// Recorre cada ruta en el array de paths.
	{
		temp_path = ft_strjoin(paths[i], "/");							// Forma una ruta temporal concatenando la ruta actual con un '/'.
		if (!temp_path)													// Si falla la creación de la ruta temporal, libera la memoria de paths y retorna NULL.
		{
			free_array(&paths);
			return (NULL);
		}
		full_path = ft_strjoin(temp_path, cmd);							// Completa la ruta agregando el comando al final de la ruta temporal.
		free(temp_path);												// Libera la memoria de la ruta temporal, ya que ahora se usa full_path.

		if (access(full_path, X_OK) == 0)								// Verifica si la ruta completa es accesible y ejecutable.
		{
			free_array(&paths);											// Si se encuentra el ejecutable, libera la memoria de paths.
			return (full_path);											// Devuelve la ruta completa del ejecutable.
		}
		free(full_path);												// Si la ruta no es válida, libera la memoria de full_path.
		i++;															// Incrementa el índice para probar la siguiente ruta en paths.
	}
	free_array(&paths);													// Si no se encuentra el ejecutable en ninguna de las rutas, libera paths.
	return (NULL);														// Devuelve NULL si no se encuentra el comando en las rutas de PATH.
}

void handle_pipes(t_cmd *cmd, t_minishell *shell)
{
	int		pipe_fds[2];												// Array de dos enteros que almacenará los descriptores de archivos para los extremos de la tubería.
	pid_t	pid;														// Variable para almacenar el ID del proceso hijo creado con fork().
	char	*command_path;												// Variable para almacenar la ruta completa del comando que se va a ejecutar.

	if (pipe(pipe_fds) == -1)											// Crea una tubería. Si falla, imprime un error y sale del programa.
	{
		perror("pipe");
		exit(EXIT_FAILURE);
	}

	pid = fork();														// Crea un nuevo proceso hijo.
	if (pid == -1)														// Si la llamada a fork falla, imprime un error y sale del programa.
	{
		perror("fork");
		exit(EXIT_FAILURE);
	}

	if (pid == 0)														// Código que ejecuta el proceso hijo.
	{
		close(pipe_fds[0]);												// Cierra el extremo de lectura de la tubería (solo escribirá en ella).
		
		if (dup2(pipe_fds[1], STDOUT_FILENO) == -1)						// Redirige la salida estándar (STDOUT) hacia la tubería (pipe_fds[1]).
		{
			perror("dup2");
			exit(EXIT_FAILURE);
		}

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
		close(pipe_fds[1]);												// Cierra el extremo de escritura de la tubería (solo leerá de ella).
		
		if (dup2(pipe_fds[0], STDIN_FILENO) == -1)						// Redirige la entrada estándar (STDIN) hacia la tubería (pipe_fds[0]).
		{
			perror("dup2");
			exit(EXIT_FAILURE);
		}
		close(pipe_fds[0]);												// Cierra el extremo de lectura de la tubería una vez redirigida la entrada.

		waitpid(pid, NULL, 0);											// El proceso padre espera a que termine el proceso hijo.
	}
}
