/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_variables_helpers.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pabloglez <pabloglez@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/30 23:22:18 by albelope          #+#    #+#             */
/*   Updated: 2024/11/04 18:37:11 by pabloglez        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char *handle_escaped_dollar(int *i)
{
	*i += 2;																// Avanza el índice 2 posiciones para omitir el escape del símbolo '$'
	return (ft_strdup("$"));												// Retorna una nueva cadena con el símbolo '$' 
}

char *handle_dollar_sign(const char *str, int *i, t_minishell *shell)
{
	char	*expanded;
	char	*var_name;
	int		start;

	(*i)++;																	// Avanza el índice para procesar el carácter después de '$'

	if (str[*i] == '?')														// Si el carácter es '?'
	{
		expanded = get_expanded_value("?", shell);							// Obtiene el valor de la variable especial '?'
		(*i)++;																// Avanza el índice después de procesar '?'
	}
	else if (ft_isalpha(str[*i]) || str[*i] == '_')							// Si el carácter es una letra o '_'
	{
		start = *i;															// Marca el inicio del nombre de la variable
		while (str[*i] && (ft_isalnum(str[*i]) || str[*i] == '_'))
			(*i)++;															// Avanza mientras haya caracteres alfanuméricos o '_'

		var_name = ft_substr(str, start, *i - start);						// Extrae el nombre de la variable
		expanded = get_expanded_value(var_name, shell);						// Expande la variable con su valor
		free(var_name);														// Libera la memoria de var_name
	}
	else
		expanded = ft_strdup("$");											// Si no es una variable, devuelve '$' como está

	return (expanded);														// Retorna la cadena expandida
}

char *handle_regular_char(const char *str, int *i)
{
	char *expanded;

	expanded = ft_substr(str, *i, 1);										// Obtiene el carácter actual como una subcadena
	(*i)++;																	// Avanza el índice después de procesar el carácter
	return (expanded);														// Retorna la cadena expandida
}

char *remove_quotes(const char *arg)
{
	size_t len;

	len = ft_strlen(arg);													// Obtiene la longitud de la cadena
	if ((arg[0] == '\'' && arg[len - 1] == '\'') ||							// Verifica si está entre comillas simples
		(arg[0] == '\"' && arg[len - 1] == '\"'))							// o comillas dobles
		return (ft_substr(arg, 1, len - 2));								// Retorna la cadena sin las comillas externas
	return (ft_strdup(arg));												// Retorna una copia de la cadena si no hay comillas
}
