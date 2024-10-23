/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pabloglez <pabloglez@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/23 19:48:11 by pabloglez         #+#    #+#             */
/*   Updated: 2024/10/23 20:14:14 by pabloglez        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	signal_handler(int signal)
{
	if (signal == SIGINT)
	{
		g_signal = 1;
		write(STDOUT_FILENO, "\n\033[1;36mminishell ➜\033[0m ", 25);
	}
}

