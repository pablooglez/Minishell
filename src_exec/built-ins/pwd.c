/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pabloglez <pabloglez@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/14 19:59:47 by pabloglez         #+#    #+#             */
/*   Updated: 2024/10/28 18:55:28 by pabloglez        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_pwd(t_minishell *shell)													//Función para manejar el comando "pwd"
{
	char	cwd[PATH_MAX];															//Buffer para almacenar el directorio actual
																					//Intenta obtener el directorio actual
	if (getcwd(cwd, sizeof(cwd)) !=  NULL)											//Si getcwd tiene éxito
		printf("%s\n", cwd);														//Imprime el directorio actual
	else
		ft_error(shell, CMD_NOT_FOUND, "pwd: failed to get current directory", 0);	//Manejo de error si getcwd falla
}