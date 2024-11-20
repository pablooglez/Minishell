/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_variables_utils.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albelope <albelope@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 20:03:33 by albelope          #+#    #+#             */
/*   Updated: 2024/11/20 20:30:52 by albelope         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*expand_argument(const char *arg, t_minishell *shell)
{
	char	*unquoted;
	char	*expanded;
	
	if (ft_strncmp(arg, "__NO_EXPAND__", 13) == 0)
	{	
		//printf("[DEBUG]-->EXPAND_ARGUMENT[0.0.0]==> No expand:           [%s]\n", arg); 
		return (ft_strdup(arg + 13));
	}
	//printf("[DEBUG]-->EXPAND_ARGUMENT[0.0.1]==> Arg:           [%s]\n", arg);
	//printf("[DEBUG]-->EXPAND_ARGUMENT[0.0.2]==> Arg[0]:           [%c]\n", arg[0]);
	//printf("[DEBUG]-->EXPAND_ARGUMENT[0.0.3]==> Arg[1]:           [%c]\n", arg[1]);
	//printf("[DEBUG]-->EXPAND_ARGUMENT[0.0.4]==> Arg[2]:           [%c]\n", arg[2]);
	if (arg[0] == '$')
	{
		expanded = expand_entire_input(arg, shell);
		if (expanded)
			return (expanded);
	}
	if (arg[0] == '\'' && arg[ft_strlen(arg) - 1] == '\'')
		return (ft_strdup(arg));
	if (arg[0] == '\"' && arg[ft_strlen(arg) - 1] == '\"')
	{
		if (ft_strchr(arg, '$') == NULL)
			return (ft_strdup(arg));
		//printf("[DEBUG]-->EXPAND_ARGUMENT[0.1.1]==> Arg before remove_quotes:         [%s]\n", arg);
		unquoted = remove_quotes(arg);
		//printf("[DEBUG]-->EXPAND_ARGUMENT[0.1.2]==> Arg after remove_quotes:          [%s]\n", unquoted);
		expanded = expand_string(unquoted, shell);
		free(unquoted);
		return (expanded);
	}
	expanded = expand_string(arg, shell);
	return (expanded);
}


