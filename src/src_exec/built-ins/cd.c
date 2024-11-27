/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pablogon <pablogon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/14 19:54:51 by pabloglez         #+#    #+#             */
/*   Updated: 2024/11/27 17:35:19 by pablogon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_cd(t_minishell *shell, char **arg)
{
	char	*dir;
	char	cwd[PATH_MAX];

	if (arg[2])
		return (ft_error(shell, MSG, "cd : too many arguments\n", 0));
	if (!arg[1] || ft_strlen(arg[1]) == 0)
	{
		dir = get_env_value(shell->env_vars, "HOME");
		if (!dir)
		{
			shell->exit_status = 1;
			return (ft_error(shell, MSG, "cd: HOME not set\n", 0));
		}
	}
	else
		dir = arg[1];
	if (chdir(dir) != 0)
	{
		shell->exit_status = 1;
		return (ft_error(shell, CD_NOT_FOUND, ft_strdup(dir), 0));
	}
	if (getcwd(cwd, sizeof(cwd)) != NULL)
		update_env_var(&(shell->env_vars), "PWD", cwd);
	return (0);
}
