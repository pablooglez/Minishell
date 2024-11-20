/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pabloglez <pabloglez@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/14 19:54:51 by pabloglez         #+#    #+#             */
/*   Updated: 2024/11/18 17:03:26 by pabloglez        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
//#include <linux/limits.h>

void	ft_cd(t_minishell *shell, char **arg)										//Función para manejar el comando "cd" (cambiar directorio)
{
	char	*dir;																	//Variable para almacenar el directorio al que se desea cambiar.
	char	cwd[PATH_MAX];															//Buffer para almacenar el directorio actual. INLCUIR LIMITS.H?

	if (arg[2])																		// Si hay más de un argumento después de 'cd' (es decir, 'cd' seguido de más de un parámetro)...
		ft_error(shell, CMD_NOT_FOUND, "cd : too many arguments", 0);				// Muestra un error diciendo "cd: too many arguments".
	
	if (!arg[1] || ft_strlen(arg[1]) == 0)											//Si no se proporciona un argumento (es decir, el usuario solo escribió 'cd') o el argumento es una cadena vacía...
	{
		dir = get_env_value(shell->env_vars, "HOME");								//Obtener el valor de la variable de entorno HOME.
		if (!dir)																	//Si no encuentra la variable HOME...
		{
			shell->exit_status = 1;
			ft_error(shell, CMD_NOT_FOUND, "cd: HOME not set", 0);					//Mostrar un error indicando que HOME no está configurado.
			return;																	//Salir de la función.
		}
	}
	else
		dir = arg[1];																//Si hay un argumento, usarlo como el directorio al que se desea cambiar.
	
	if (chdir(dir) != 0)															//Intentar cambiar al directorio especificado.
	{
		ft_error(shell, CMD_NOT_FOUND, dir, 0);										//Si falla, mostrar un error indicando que no se puede cambiar al directorio.
		return;																		//Salir de la función
	}
	if (getcwd(cwd, sizeof(cwd)) != NULL)											//Obtener el directorio actual y almacenarlo en cwd.
		update_env_var(&(shell->env_vars), "PWD", cwd);								//Actualizar la variable de entorno PWD en la lista de variables.
	else 
	{
		shell->exit_status = 1;
		ft_error(shell, CMD_NOT_FOUND, "cd: failed to get current directory", 0);	//Mostrar un error si falla getcwd().
	}
}
