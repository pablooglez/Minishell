/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_variables_utils.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albelope <albelope@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 20:03:33 by albelope          #+#    #+#             */
/*   Updated: 2024/10/29 21:34:10 by albelope         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

/*
** Maneja el caso de un dólar escapado (\$) devolviendo un '$' literal.
*/
char	*handle_escaped_dollar(int *i)
{
	*i += 2;
	return (ft_strdup("$"));
}

/*
** Maneja variables de entorno y el caso especial de '$?'.
*/
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

/*
** Maneja caracteres regulares no relacionados con variables.
*/
char	*handle_regular_char(const char *str, int *i)
{
	char	*expanded;

	expanded = ft_substr(str, *i, 1);
	(*i)++;
	return (expanded);
}

/*
** Elimina las comillas iniciales y finales de un argumento si las tiene.
*/
char	*remove_quotes(const char *arg)
{
	size_t	len;

	len = ft_strlen(arg);
	if ((arg[0] == '\'' && arg[len - 1] == '\'') ||
		(arg[0] == '\"' && arg[len - 1] == '\"'))
		return (ft_substr(arg, 1, len - 2));
	return (ft_strdup(arg));
}

/*
** Expande un argumento individual, manejando las comillas y variables.
*/
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