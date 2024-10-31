/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_variables_utils.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albelope <albelope@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 20:03:33 by albelope          #+#    #+#             */
/*   Updated: 2024/10/31 15:47:08 by albelope         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"


char	*expand_argument(const char *arg, t_minishell *shell)
{
	char	*unquoted;
	char	*expanded;

	if (arg[0] == '$')
	{
		expanded = expand_entire_input(arg, shell);
		if (expanded)
			return expanded;
	}
	if (arg[0] == '\'' && arg[ft_strlen(arg) - 1] == '\'')
	{
		unquoted = remove_quotes(arg);
		return (unquoted); // En comillas simples, no se expande
	}
	else if (arg[0] == '\"' && arg[ft_strlen(arg) - 1] == '\"')
	{
		unquoted = remove_quotes(arg);
		expanded = expand_string(unquoted, shell);
		free(unquoted);
		return (expanded);
	}
	else
		return (expand_string(arg, shell));
}
