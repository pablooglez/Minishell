/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/27 17:35:48 by pablogon          #+#    #+#             */
/*   Updated: 2024/11/30 19:47:22 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	print_nl(t_minishell *shell, int new_line)
{
	if (new_line != 0)
		printf("\n");
	fflush(stdout);
	shell->exit_status = 0;
}

void	ft_echo(t_minishell *shell, char **arg)
{
	int		new_line;
	int		i;
	int		j;
	bool	is_text;

	new_line = 1;
	is_text = false;
	i = 0;
	while (arg[++i])
	{
		j = 0;
		if (!is_text && arg[i] && arg[i][j] == '-')
			while (arg[i][++j])
				if (arg[i][j] != 'n')
					break ;
		if (!is_text && arg[i] && !arg[i][j])
			new_line = 0;
		else
		{
			is_text = printf("%s", arg[i]);
			if (arg[i] && arg[i + 1])
				printf(" ");
		}
	}
	print_nl(shell, new_line);
}
