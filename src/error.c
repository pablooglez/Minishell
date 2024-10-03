/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pablogon <pablogon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/01 19:55:18 by pablogon          #+#    #+#             */
/*   Updated: 2024/10/03 19:51:52 by pablogon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_error(char *msg, t_minishell *shell, int exit_code)
{
	write (2, msg, ft_strlen(msg));
	write (2, "\n", 1);

	shell->exit_status = exit_code;		//Actualizar estado de salida del shell en el código erroneo

	if (exit_code != 0)					//Si el error es crítico, limpiar recursos y salir
	{
		cleanup_minishell(shell);		//Limpiar recursos si es necesario
		exit(exit_code);				//Terminar el programa
	}

}