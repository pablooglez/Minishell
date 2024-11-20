/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_variables_utils.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albelope <albelope@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 20:03:33 by albelope          #+#    #+#             */
/*   Updated: 2024/11/19 21:38:58 by albelope         ###   ########.fr       */
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
		//printf("[DEBUG]-->EXPAND_ARGUMENT[0.1]==> Expanded:           [%s]\n", expanded);
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
	//printf("[DEBUG]-->EXPAND_ARGUMENT[0.2]==>Arg bef.. expand_string:         [%s]\n", arg);
	expanded = expand_string(arg, shell);
	//printf("[DEBUG]-->EXPAND_ARGUMENT[0.3]==>Expand later expand_string:     [%s]\n", expanded);	
	return (expanded);
}


