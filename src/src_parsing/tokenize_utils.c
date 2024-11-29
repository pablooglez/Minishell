/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albelope <albelope@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/29 22:54:12 by albelope          #+#    #+#             */
/*   Updated: 2024/11/29 22:56:58 by albelope         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	handle_dollar_case(char *buffer, int *buf_index, t_minishell *shell)
{
	char	*pid;
	int		k;

	(void)shell;
	pid = ft_itoa(getpid());
	if (!pid)
		return (-1);
	k = 0;
	while (pid[k])
		buffer[(*buf_index)++] = pid[k++];
	free(pid);
	return (0);
}

int	get_variable_name(char *input, char *var_name, int *var_len,
	t_minishell *shell)
{
	while (input[shell->i] && (ft_isalnum(input[shell->i])
			|| input[shell->i] == '_' || input[shell->i] == '?'))
		var_name[(*var_len)++] = input[shell->i++];
	var_name[*var_len] = '\0';
	return (0);
}

int	copy_variable_value(char *var_name, char *buffer, int *buf_index,
	t_minishell *shell)
{
	char	*value;
	int		k;

	if (ft_strncmp(var_name, "?", 1) == 0)
		value = ft_itoa(shell->exit_status);
	else
		value = ft_strdup(get_env_value(shell->env_vars, var_name));
	if (!value)
		return (0);
	k = 0;
	while (value[k])
		buffer[(*buf_index)++] = value[k++];
	free(value);
	return (1);
}
