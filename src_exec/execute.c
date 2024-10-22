/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pabloglez <pabloglez@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/09 16:59:29 by pabloglez         #+#    #+#             */
/*   Updated: 2024/10/22 19:50:37 by pabloglez        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	execute_command(t_minishell *shell)
{
	t_cmd	*cmd;															//Inicializa un puntero a la lista de comandos (tokens) en la estructura del shell

	cmd = shell->tokens;
	
	while(cmd)																//Comienza un bucle que continua hasta que haya comandos para procesar
	{
		printf("(EXECUTE_COMMAND())	Comando a ejecutar: 	%s\n", cmd->arguments[0]);
		if (handle_builtin(cmd, shell))										//Verificamos primero si se trata de un comando especial(built-ins[echo, cd, pwd, export, unset, env, exit]);
		{ 									
			printf("(EXECUTE_COMMAND())     Ejecutando built-in:     %s\n", cmd->path);
			return (0);														//Si es un comando interno, sale de la función sin terminar
		}
		if (cmd->next && cmd->next->type == PIPE)							//Comprueba si existe luego un comando y si es un PIPE
			handle_pipes(cmd, shell);												//Maneja la ejecución de comandos conectados por pipes
		else																//Si no hay pipe después del comando actual...
		{
			pid_t pid = fork();												//Creamos un nuevo proceso hijo usando fork()
			if (pid == 0)													//Verificamos si estamos en el proceso hijo
			{
				//handle_redirection(cmd);									//Maneja las redirecciones de entrada y de salida
				if (execvp(cmd->arguments[0], cmd->arguments) == -1)		//Ejecutamos el comando con execvpe
					ft_error(shell, CMD_NOT_FOUND, cmd->arguments[0], 1);	//Manejar el error si falla execvp()
			}
			else if (pid < 0)												//Verifica sin hubo un error al crear el proceso
				ft_error(shell, MEMORY, NULL, 0);							//Maneja el error de memoria si falla fork()
			else
				waitpid(pid, &shell->exit_status, 0);										//Esperar a que el proceso hijo termine
			if (WIFEXITED(shell->exit_status))										//Verificar si el proceso terminó normalmente
					shell->exit_status = WEXITSTATUS(shell->exit_status);					//Obtener el código de salida
				else if (WIFSIGNALED(shell->exit_status))								//Verificar si el proceso fue terminado por una señal
					shell->exit_status = 128 + WTERMSIG(shell->exit_status);					//Obtener el número de la señal
		}
		cmd = cmd->next;													//Avanza al siguiente comando en la lista
	}
	return (0);																//Retorna 0 al finalizar la ejecución
}
