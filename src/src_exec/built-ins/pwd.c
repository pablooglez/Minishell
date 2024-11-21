/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pabloglez <pabloglez@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/14 19:59:47 by pabloglez         #+#    #+#             */
/*   Updated: 2024/11/21 19:42:06 by pabloglez        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_pwd(t_minishell *shell)													//Función para manejar el comando "pwd"
{
	char	cwd[PATH_MAX];															//Buffer para almacenar el directorio actual
																					//Intenta obtener el directorio actual
	if (getcwd(cwd, sizeof(cwd)) !=  NULL)											//Si getcwd tiene éxito
		printf("%s\n", cwd);														//Imprime el directorio actual
	else
		return (ft_error(shell, MSG, "pwd: failed to get current directory\n", 0));	//Manejo de error si getcwd falla
	return (0);
}