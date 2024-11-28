/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pablogon <pablogon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/27 17:35:48 by pablogon          #+#    #+#             */
/*   Updated: 2024/11/27 17:39:47 by pablogon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void print_nl(int new_line)
{
    if (new_line != 0)
	    printf("\n");
    fflush(stdout);
}

void	ft_echo(char **arg)
{
	int     new_line;
	int     i;
    int     j;
    bool    is_text;

	new_line = 1;
    is_text = false;
	i = 0;
    while (arg[++i])
    {
        j = 0;
        if (!is_text && arg[i] && arg[i][j] == '-')
            while (arg[i][++j])
                if (arg[i][j] != 'n')
                    break;
        if (!is_text && arg[i] && !arg[i][j])
            new_line = 0;
        else
        {
            is_text = printf("%s",  arg[i]);
		    if (arg[i] && arg[i + 1])
			    printf(" ");
        }
	}
    print_nl(new_line);
}
