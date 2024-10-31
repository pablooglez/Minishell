/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_variables_helpers.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albelope <albelope@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/30 23:22:18 by albelope          #+#    #+#             */
/*   Updated: 2024/10/30 23:22:40 by albelope         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

char	*handle_escaped_dollar(int *i)
{
	*i += 2;
	return (ft_strdup("$"));
}

char	*handle_dollar_sign(const char *str, int *i, t_minishell *shell)
{
	char	*expanded;
	char	*var_name;
	int		start;

	(*i)++;
	if (str[*i] == '?')
	{
		expanded = get_expanded_value("?", shell);
		(*i)++;
	}
	else if (ft_isalpha(str[*i]) || str[*i] == '_')
	{
		start = *i;
		while (str[*i] && (ft_isalnum(str[*i]) || str[*i] == '_'))
			(*i)++;
		var_name = ft_substr(str, start, *i - start);
		expanded = get_expanded_value(var_name, shell);
		free(var_name);
	}
	else
		expanded = ft_strdup("$");
	return (expanded);
}

char	*handle_regular_char(const char *str, int *i)
{
	char	*expanded;

	expanded = ft_substr(str, *i, 1);
	(*i)++;
	return (expanded);
}


char	*remove_quotes(const char *arg)
{
	size_t	len;

	len = ft_strlen(arg);
	if ((arg[0] == '\'' && arg[len - 1] == '\'') ||
		(arg[0] == '\"' && arg[len - 1] == '\"'))
		return (ft_substr(arg, 1, len - 2));
	return (ft_strdup(arg));
}