/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_variables_utils.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pabloglez <pabloglez@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 20:03:33 by albelope          #+#    #+#             */
/*   Updated: 2024/11/04 19:12:07 by pabloglez        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char *expand_argument(const char *arg, t_minishell *shell)
{
	char	*unquoted;
	char	*expanded;

	if (arg[0] == '$')												// Si el argumento comienza con '$'
	{
		expanded = expand_entire_input(arg, shell);					// Expande todo el argumento como una variable
		if (expanded)
			return expanded;										// Si se expande correctamente, retorna el resultado
	}
	if (arg[0] == '\'' && arg[ft_strlen(arg) - 1] == '\'')			// Si el argumento está entre comillas simples
	{
		unquoted = remove_quotes(arg);								// Remueve las comillas simples
		return (unquoted);											// Retorna la cadena sin expandir, ya que no se expande entre comillas simples
	}
	else if (arg[0] == '\"' && arg[ft_strlen(arg) - 1] == '\"')		// Si el argumento está entre comillas dobles
	{
		unquoted = remove_quotes(arg);								// Remueve las comillas dobles
		expanded = expand_string(unquoted, shell);					// Expande la cadena sin las comillas
		free(unquoted);												// Libera la memoria de la cadena sin comillas
		return (expanded);											// Retorna la cadena expandida
	}
	else
		return (expand_string(arg, shell));							// Si no hay comillas, expande directamente el argumento
}

