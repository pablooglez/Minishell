/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_variables_utils.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albelope <albelope@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 20:03:33 by albelope          #+#    #+#             */
/*   Updated: 2024/11/29 21:56:11 by albelope         ###   ########.fr       */
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
	expanded = expand_string(arg, shell);
	return (expanded);
}

char	*expand_entire_input(const char *input, t_minishell *shell)
{
	char	*expanded;

	expanded = expand_string(input, shell);
	if (expanded != NULL && access(expanded, X_OK) == 0)
		return (expanded);
	free(expanded);
	return (NULL);
}

void	expand_tokens(t_cmd *cmd, t_minishell *shell)
{
	int		i;
	char	*expanded;

	i = 0;
	while (cmd->arguments && cmd->arguments[i] != NULL)
	{
		expanded = expand_argument(cmd->arguments[i], shell);
		free(cmd->arguments[i]);
		cmd->arguments[i] = expanded;
		i++;
	}
}
