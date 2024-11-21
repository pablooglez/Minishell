/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pabloglez <pabloglez@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/09 16:59:29 by pabloglez         #+#    #+#             */
/*   Updated: 2024/11/21 19:35:33 by pabloglez        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int count_nodes(t_env *env_vars)
{
    int	count;

	count = 0;
    while (env_vars)
    {
        count++;
        env_vars = env_vars->next;
    }
    return (count);
}

char **free_env_array(char ***env_array, int i)
{
	while (i > -1)
		free((*env_array)[i--]);
    free(*env_array);
    return (NULL);
}

char **env_vars_to_array(t_env *env_vars)
{
	char	**env_array;
	int		len;
	int		i;

    env_array = malloc((count_nodes(env_vars) + 1) * sizeof(char *));
    if (!env_array)
        return (NULL);
    i = 0;
    while (env_vars)
    {
        len = ft_strlen(env_vars->key) + 1;
        if (env_vars->value)
            len += ft_strlen(env_vars->value);
        env_array[i] = malloc((len + 1) * sizeof(char));
        if (!env_array[i])
			return (free_env_array(&env_array, i));
		env_array[i] = ft_strjoin2(env_vars->key, "=", env_vars->value);
		if (!env_array[i])
			return (free_env_array(&env_array, --i));
        env_vars = env_vars->next;
        i++;
    }
    env_array[i] = NULL;
    return (env_array);
}



static void	execute_child(t_minishell *shell, t_cmd	*cmd) {
	char	*command_path;																	// Declaración de un puntero que contendrá la ruta completa del comando a ejecutar

	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	safe_dup2(cmd->pipe[0], STDIN_FILENO);
	safe_dup2(cmd->pipe[1], STDOUT_FILENO);
	safe_close(cmd->intfd);
	if (handle_builtin(cmd, shell))
		exit(shell->exit_status);
	handle_redirection(shell, cmd->redir, -1);
	command_path = get_command_path(cmd->arguments[0], shell);					// Llama a get_command_path para buscar el comando en los directorios de PATH
	if (!command_path)															// Si el comando no se encuentra, maneja el error
		ft_error(shell, CMD_NOT_FOUND, cmd->arguments[0], 1);					// Imprime un error indicando que el comando no se encuentra
	if (execve(command_path, cmd->arguments, env_vars_to_array(shell->env_vars)) == -1)					// Si execve falla, imprime un mensaje de error
	{
		ft_error(shell, CMD_NOT_FOUND, cmd->arguments[0], 1);					// Imprime un mensaje de error si execve falla
		free(command_path);															// Libera la memoria de command_path si execve se ejecuta correctamente
	}
}

static void	execute_commands(t_minishell *shell)
{
	t_cmd	*cmd;																			// Inicializa un puntero a la lista de comandos (tokens) en la estructura del shell

	cmd = shell->tokens;																	// Asigna el inicio de la lista de comandos a la variable cmd
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
	while (cmd)																				// Comienza un bucle que continúa mientras haya comandos para procesar
	{
		if (handle_pipe(cmd) == 1)
			ft_error(shell, MEMORY_ERROR, NULL, 1);
		cmd->pid = fork();																	// Crea un nuevo proceso hijo con fork()
		if (cmd->pid < 0)																// Si fork() falla, verifica que pid sea menor que 0 (indica un error)
			ft_error(shell, MEMORY_ERROR, NULL, 1);											// Si hubo un error al crear el proceso, maneja el error de memoria
		else if (cmd->pid == 0)																	// Verifica si el proceso actual es el hijo (pid == 0 significa proceso hijo)
			execute_child(shell, cmd);
		safe_close(cmd->pipe[0]);
		safe_close(cmd->pipe[1]);
		cmd = cmd->next;																	// Avanza al siguiente comando en la lista de comandos
	}
}

void	execute(t_minishell *shell)
{
	t_cmd	*cmd;

	cmd = shell->tokens;
	if ((!cmd->prev || cmd->prev->type != 3) && cmd->type != 3 && handle_builtin(cmd, shell))								// Verifica si el comando es un built-in (comandos internos como echo, cd, etc.)
	{
		dup2(shell->original_stdin, STDIN_FILENO);
		dup2(shell->original_stdout, STDOUT_FILENO);
	}
	else
	{
		execute_commands(shell);
		while (cmd)
		{
			if (cmd->pid != -1)
			{
				waitpid(cmd->pid, &shell->exit_status, 0);										// Espera a que el proceso hijo termine su ejecución
				if (WIFEXITED(shell->exit_status))											// Si el proceso terminó normalmente (sin señal)
					shell->exit_status = WEXITSTATUS(shell->exit_status);					// Obtiene el código de salida del proceso hijo
				else if (WIFSIGNALED(shell->exit_status))									// Si el proceso terminó debido a una señal
					shell->exit_status = 128 + WTERMSIG(shell->exit_status);				// Obtiene el número de la señal que lo finalizó
			}
			cmd = cmd->next;
		}
	}
	if (shell->exit_status >= 128)
		write(1, "\n", 1);
	signal(SIGINT, signal_handler);                                         // Configura el manejador de señal para SIGINT
	signal(SIGQUIT, SIG_IGN);                                               // Ignora SIGQUIT para evitar que el shell termine
}

