/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_variables_helpers.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albelope <albelope@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/30 23:22:18 by albelope          #+#    #+#             */
/*   Updated: 2024/11/13 21:19:28 by albelope         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


char	*handle_escaped_dollar(int *i)
{
	//printf("[DEBUG]-->HANDLE_ESCAPED_DOLLAR[0.0]==> Start:                     [%d]\n", *i);
	*i += 2;
	//printf("[DEBUG]-->HANDLE_ESCAPED_DOLLAR[0.0]==> Start despues de *i+=2:                   [%d]\n", *i);
	return (ft_strdup("$"));
}
char	*handle_special_cases(const char *str, int *i, t_minishell *shell)
{
	char	*expanded;															// Variable para almacenar la cadena expandida

	if (str[*i] == '?')															// Verifica si el carácter actual es '?'
	{
		(*i)++;																	// Incrementa el índice para pasar al siguiente carácter
		expanded = get_expanded_value("?", shell); 								// Obtiene el valor expandido de la variable '?'
		if (!expanded) 															// Verifica si la expansión falla
			return (ft_strdup("")); 											// Retorna una cadena vacía si falla
		return (expanded); 														// Retorna la cadena expandida si tiene éxito
	}
	if (str[*i] == '$')  														// Verifica si es $
	{
		(*i)++;																	// Incrementa el índice para pasar al siguiente carácter
		return (ft_itoa(getpid())); 											// Retorna el PID del proceso actual como una cadena
	}
	if (ft_isdigit(str[*i]))													// Verifica si el carácter actual es un dígito
	{
		while (ft_isdigit(str[*i]))												// Recorre los dígitos
			(*i)++;																// Incrementa el índice para pasar al siguiente carácter
		return (ft_strdup(""));													// Retorna una cadena vacía
	}
	return (NULL);																// Retorna NULL si no se cumplen las condiciones anteriores
}

char	*handle_dollar_sign(const char *str, int *i, t_minishell *shell)
{
	char	*expanded;															// Variable para almacenar la cadena expandida
	char	*var_name;														    // Variable para almacenar el nombre de la variable			
	int		start; 																// Variable para almacenar la posición inicial de la variable

	(*i)++; 																	// Incrementa el índice para pasar al siguiente carácter
	expanded = handle_special_cases(str, i, shell); 							// Maneja los casos especiales
	if (expanded != NULL)
		return (expanded); 														// Retorna la cadena expandida si no es NULL
	if (ft_isalpha(str[*i]) || str[*i] == '_') 									// Verifica si el carácter actual es alfabético o un guion bajo	
	{
		start = *i; 															// Almacena la posición inicial de la variable
		while (str[*i] && (ft_isalnum(str[*i]) || str[*i] == '_')) 				// Recorre los caracteres alfanuméricos y guiones bajos
			(*i)++; 															// Recorre los caracteres alfanuméricos y guiones bajos
		var_name = ft_substr(str, start, *i - start); 							// Obtiene el nombre de la variable
		if (var_name == NULL) 													// Verifica si la obtención del nombre falla
			return (ft_strdup("")); 											// Retorna una cadena vacía si falla
		expanded = get_expanded_value(var_name, shell); 						// Obtiene el valor expandido de la variable
		free(var_name); 														// Libera la memoria del nombre de la variable
		if (expanded == NULL) 													// Verifica si la expansión falla
			return (ft_strdup("")); 											// Retorna una cadena vacía si falla
		return (expanded); 														// Retorna la cadena expandida si tiene éxito
	}
	return (ft_strdup("$")); 													// Retorna '$' si no se cumplen las condiciones anteriores
}

char	*handle_regular_char(const char *str, int *i)
{
	char	*expanded;
	
	//printf("[DEBUG]-->HANDLE_REGULAR_CHAR[0.0]==> Start:                       [%d]\n", *i);
	expanded = ft_substr(str, *i, 1);
	(*i)++;
	//printf("[DEBUG]-->HANDLE_REGULAR_CHAR[0.1]==> Expanded:                    [%s]\n", expanded);
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
