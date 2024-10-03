/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pablogon <pablogon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/01 19:55:18 by pablogon          #+#    #+#             */
/*   Updated: 2024/10/03 21:58:19 by pablogon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void fatal(int code, char *value) 
{
	if (code == MEMORY)
		printf("MEMORIA\n");
	if (code == CMD_NOT_FOUND)
		printf("CMD %s\n", value);
}

void	ft_error(t_minishell *shell, int code, char * value)
{
	fatal(code, value);

	if (value) free(value);
	//cleanup_minishell(shell);		//Limpiar recursos si es necesario
	exit(shell->exit_status);				//Terminar el programa

}