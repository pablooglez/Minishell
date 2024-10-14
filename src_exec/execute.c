/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pabloglez <pabloglez@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/09 16:59:29 by pabloglez         #+#    #+#             */
/*   Updated: 2024/10/14 18:28:04 by pabloglez        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	execute_command(t_minishell *shell)
{
	t_cmd	*cmd;															//Inicializa un puntero a la lista de comandos (tokens) en la estructura del shell

	cmd = shell->tokens;
	
	while(cmd)																//Comienza un bucle que continua hasta que haya comandos para procesar
	{
		if (handle_builtin(cmd, shell))										//Verificamos primero si se trata de un comando especial(built-ins[echo, cd, pwd, export, unset, env, exit]);
			return (0);														//Si es un comando interno, sale de la función sin terminar
		if (cmd->next && cmd->next->type == PIPE)							//Comprueba si existe luego un comando y si es un PIPE
			handle_pipes(cmd);												//Maneja la ejecución de comandos conectados por pipes
		else																//Si no hay pipe después del comando actual...
		{
			pid_t pid = fork();												//Creamos un nuevo proceso hijo usando fork()
			if (pid == 0)													//Verificamos si estamos en el proceso hijo
			{
				handle_redirection(cmd);									//Maneja las redirecciones de entrada y de salida
				if (execvp(cmd->arguments[0], cmd->arguments) == -1)		//Ejecutamos el comando con execvpe
					ft_error(shell, CMD_NOT_FOUND, cmd->arguments[0], 1);	//Manejar el error si falla execvp()
			}
			else if (pid < 0)												//Verifica sin hubo un error al crear el proceso
				ft_error(shell, MEMORY, NULL, 0);							//Maneja el error de memoria si falla fork()
			else
				waitpid(pid, NULL, 0);										//Esperar a que el proceso hijo termine
		}
		cmd = cmd->next;													//Avanza al siguiente comando en la lista
	}
	return (0);																//Retorna 0 al finalizar la ejecución
}
