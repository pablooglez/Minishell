/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pabloglez <pabloglez@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/23 19:48:11 by pabloglez         #+#    #+#             */
/*   Updated: 2024/11/20 19:46:55 by pabloglez        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	signal_handler(int signal)
{
	if (signal == SIGINT)													// Manejo de SIGINT (Ctrl + C)
	{
		g_signal = 1;														// Indica que se recibió la señal
		write (1, "\033[K\n", 5);
		//rl_replace_line("", 0);											// Borra la línea actual en la entrada de readline.
	}
	else
	{
		rl_on_new_line();
		rl_redisplay();														// Actualiza el prompt para mostrar una línea limpia.
		write (1, "\033[K\n", 5);
		rl_on_new_line();
		rl_redisplay();
	}
}

