/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pablogon <pablogon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/23 19:48:11 by pabloglez         #+#    #+#             */
/*   Updated: 2024/11/28 01:09:02 by pablogon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	signal_handler(int sig)
{
	g_signal = sig;
	rl_on_new_line();
	rl_redisplay();
	write (1, "\n", 1);
	rl_on_new_line();
	rl_redisplay();
}
