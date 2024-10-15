/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built-ins.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pabloglez <pabloglez@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/14 19:15:09 by pabloglez         #+#    #+#             */
/*   Updated: 2024/10/14 20:57:05 by pabloglez        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	handle_builtin(t_cmd *cmd, t_minishell *shell)
{
	if (ft_strncmp(cmd->arguments[0], "echo", 5) == 0)				//Comprobamos si el comando es "echo"
	{
		ft_echo(shell, cmd->arguments);								//Llama a la función personalizada para "echo"
		return (1);													//Retorna 1 para indicar que es un comando builtin
	}
	else if (ft_strncmp(cmd->arguments[0], "cd", 3) == 0)			//Comprobamos si el comando es "cd"
	{
		ft_cd(shell, cmd->arguments);								//Llama a la función personalizada para "cd"
		return (1);													//Retorna 1 para indicar que es un comando builtin
	}
	else if (ft_strcmp(cmd->arguments[0], "pwd", 4) == 0)			//Comprobamos si el comando es "pwd"
	{
		ft_pwd(shell);												//Llama a la función personalizada para "pwd"
		return (1);													//Retorna 1 para indicar que es un comando builtin
	}
	else if(ft_strncmp(cmd->arguments[0], "export", 7) == 0)		//Comprobamos si el comando es "export"
	{
		ft_export(shell, cmd->arguments);							//Llama a la función personalizada para "export"
		return (1);													//Retorna 1 para indicar que es un comando builtin
	}
	else if (ft_strncmp(cmd->arguments[0], "unset", 6) == 0)		//Comprobamos si el comando es "unset"
	{
		ft_unset(shell, cmd->arguments);							//Llama a la función personalizada para "unset"
		return (1);													//Retorna 1 para indicar que es un comando builtin
	}
	else if (ft_strncmp(cmd->arguments[0], "env", 4) == 0)			//Comprobamos si el comando es "env"
	{
		ft_env(shell);												//Llama a la función personalizada para "env"
		return (1);													//Retorna 1 para indicar que es un comando builtin
	}
	else if (ft_strncmp(cmd->arguments[0], "exit", 5) == 0)			//Comprobamos si el comando es "exit"
	{
		exit_shell(shell);											//Llama a la función personalizada para "exit"
		return (1);													//Retorna 1 para indicar que es un comando builtin
	}
	return (0);														//Si no es un built-in, retornamos 0
}

