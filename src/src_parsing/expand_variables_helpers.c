/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_variables_helpers.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pablogon <pablogon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/30 23:22:18 by albelope          #+#    #+#             */
/*   Updated: 2024/11/28 19:09:43 by pablogon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*handle_escaped_dollar(int *i)
{
	*i += 2;
	return (ft_strdup("$"));
}

char	*handle_special_cases(const char *str, int *i, t_minishell *shell)
{
	char	*expanded;

	if (str[*i] == '?')
	{
		(*i)++;
		expanded = get_expanded_value("?", shell);
		if (!expanded)
			return (ft_strdup(""));
		return (expanded);
	}
	if (str[*i] == '$')
	{
		(*i)++;
		return (ft_itoa(getpid()));
	}
	if (ft_isdigit(str[*i]))
	{
		while (ft_isdigit(str[*i]))
			(*i)++;
		return (ft_strdup(""));
	}
	return (NULL);
}

char	*handle_dollar_sign(const char *str, int *i, t_minishell *shell)
{
	char	*expanded;
	char	*var_name;
	int		start;

	(*i)++;
	expanded = handle_special_cases(str, i, shell);
	if (expanded != NULL)
		return (expanded);
	if (ft_isalpha(str[*i]) || str[*i] == '_')
	{
		start = *i;
		while (str[*i] && (ft_isalnum(str[*i]) || str[*i] == '_'))
			(*i)++;
		var_name = ft_substr(str, start, *i - start);
		if (var_name == NULL)
			return (ft_strdup(""));
		expanded = get_expanded_value(var_name, shell);
		free(var_name);
		if (expanded == NULL)
			return (ft_strdup(""));
		return (expanded);
	}
	return (ft_strdup("$"));
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
	if ((arg[0] == '\'' && arg[len - 1] == '\'')
		|| (arg[0] == '\"' && arg[len - 1] == '\"'))
		return (ft_substr(arg, 1, len - 2));
	return (ft_strdup(arg));
}
