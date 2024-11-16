/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_variables_utils.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albelope <albelope@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 20:03:33 by albelope          #+#    #+#             */
/*   Updated: 2024/11/16 13:36:39 by albelope         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*expand_argument(const char *arg, t_minishell *shell)
{
	char	*unquoted;
	char	*expanded;

	if (ft_strncmp(arg, "__NO_EXPAND__", 13) == 0)
		return (ft_strdup(arg + 13));
	if (arg[0] == '$')
	{
		expanded = expand_entire_input(arg, shell);
		//printf("DEBUG]-->EXPAND_ARGUMENT[0.1]==> Expanded:           [%s]\n", expanded);
		if (expanded)
			return (expanded);
	}
	if (arg[0] == '\'' && arg[ft_strlen(arg) - 1] == '\'')
		return (ft_strdup(arg));
	if (arg[0] == '\"' && arg[ft_strlen(arg) - 1] == '\"')
	{
		if (ft_strchr(arg, '$') == NULL)
			return (ft_strdup(arg));
		unquoted = remove_quotes(arg);
		expanded = expand_string(unquoted, shell);
		free(unquoted);
		return (expanded);
	}
	//printf("DEBUG]-->EXPAND_ARGUMENT[0.2]==>Arg bef.. expand_string:         [%s]\n", arg);
	expanded = expand_string(arg, shell);
	//printf("DEBUG]-->EXPAND_ARGUMENT[0.3]==>Expand later expand_string:     [%s]\n", expanded);	
	return (expanded);
}


