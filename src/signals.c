/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pabloglez <pabloglez@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/23 19:48:11 by pabloglez         #+#    #+#             */
/*   Updated: 2024/11/19 21:07:26 by pabloglez        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <sys/ioctl.h>
void	signal_handler(int signal)
{
	if (signal == SIGINT)												// Manejo de SIGINT (Ctrl + C)
	{
		g_signal = 1;													// Indica que se recibió la señal
		ioctl(STDIN_FILENO, TIOCSTI, "\n");
		rl_on_new_line();
		// write(STDOUT_FILENO, "\n", 1);	// Imprime un salto de línea y el prompt
		// rl_on_new_line();												// Marca la nueva línea como el inicio de la entrada.
		// //rl_replace_line("", 0);										// Borra la línea actual en la entrada de readline.
		// rl_redisplay();													// Actualiza el prompt para mostrar una línea limpia.
	}
}

