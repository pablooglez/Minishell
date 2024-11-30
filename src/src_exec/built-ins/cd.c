/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/14 19:54:51 by pabloglez         #+#    #+#             */
/*   Updated: 2024/11/30 19:52:51 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	update_vars(t_minishell *shell)
{
	char	cwd[PATH_MAX];

	update_env_var(&shell->env_vars, "OLDPWD",
		get_env_value(shell->env_vars, "PWD"));
	if (getcwd(cwd, sizeof(cwd)) != NULL)
		update_env_var(&shell->env_vars, "PWD", cwd);
}

static char	*special_paths(t_minishell *shell, char **arg)
{
	char	*dir;

	dir = NULL;
	if (!arg[1] || !arg[1][0])
	{
		dir = get_env_value(shell->env_vars, "HOME");
		if (!dir)
		{
			shell->exit_status = 1;
			ft_error(shell, MSG, "cd: HOME not set\n", 0);
		}
	}
	else if (!ft_strcmp(arg[1], "-"))
	{
		dir = get_env_value(shell->env_vars, "OLDPWD");
		if (!dir)
		{
			shell->exit_status = 1;
			ft_error(shell, MSG, "cd: OLDPWD not set\n", 0);
		}
	}
	return (dir);
}

static int	change_dir(t_minishell *shell, char *dir)
{
	char	*home;
	bool	free_dir;

	free_dir = false;
	if (dir && dir[0] == '~')
	{
		home = get_env_value(shell->env_vars, "HOME");
		if (home)
		{
			dir = ft_strjoin(home, dir + 1);
			free_dir = true;
		}
	}
	if (chdir(dir) != 0)
	{
		shell->exit_status = 1;
		if (free_dir)
			free(dir);
		return (ft_error(shell, CD_NOT_FOUND, ft_strdup(dir), 0));
	}
	if (free_dir)
		free(dir);
	update_vars(shell);
	return (0);
}

int	ft_cd(t_minishell *shell, char **arg)
{
	char	*dir;

	if (arg[1] && arg[2])
	{
		shell->exit_status = 1;
		return (ft_error(shell, MSG, "cd : too many arguments\n", 0));
	}
	dir = special_paths(shell, arg);
	if (!dir && arg[1] && arg[1][0])
		dir = arg[1];
	if (!dir || change_dir(shell, dir))
		shell->exit_status = 1;
	else
		shell->exit_status = 0;
	return (0);
}
