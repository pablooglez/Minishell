/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_variables.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albelope <albelope@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/17 22:02:04 by albelope          #+#    #+#             */
/*   Updated: 2024/10/25 13:23:23 by albelope         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

/*
** Obtiene el valor expandido de una variable de entorno.
** Si la variable es "$?", devuelve el código de salida del último comando ejecutado.
** Si la variable está en el entorno, devuelve su valor expandido.
*/
static char	*get_expanded_value(const char *variable, int last_exit_status)
{
	char	*expanded;

	if (ft_strncmp(variable, "?", 2) == 0)                                                 // Si la variable es "$?"
		return (ft_itoa(last_exit_status));                                                // Devuelve el código de salida como string
	expanded = getenv(variable);                                                           // Busca la variable en el entorno
	if (expanded)                                                                          // Si la variable existe en el entorno
		return (ft_strdup(expanded));                                                      // Devuelve una copia de su valor
	return (ft_strdup(""));                                                                // Si no existe, devuelve una cadena vacía
}

/*
** Expande variables, "$?" y caracteres regulares.
** Identifica variables de entorno a partir del símbolo `$` y realiza la expansión,
** manejando el caso especial de `$?` para el código de salida del último comando.
*/
static char	*expand_var_or_char(const char *str, int *i, t_minishell *shell)
{
	char	*expanded;
	int		start;

	if (str[*i] == '$' && (ft_isalnum(str[*i + 1]) || str[*i + 1] == '_'))                 // Detecta una variable de entorno
	{
		start = *i + 1;                                                                    // Marca el inicio de la variable
		while (str[start] && (ft_isalnum(str[start]) || str[start] == '_'))                // Recorre hasta el final de la variable
			start++;
		expanded = get_expanded_value(ft_substr(str, *i + 1, start - (*i + 1)),            // Expande la variable usando su nombre
									   shell->exit_status);
		*i = start;                                                                       // Actualiza el índice a la posición final de la variable
	}
	else if (str[*i] == '$' && str[*i + 1] == '?')                                         // Maneja el caso especial de `$?`
	{
		expanded = get_expanded_value("?", shell->exit_status);                            // Expande el código de salida
		*i += 2;                                                                          // Salta `$?` en el índice
	}
	else                                                                                   // Si no es una variable, trata como carácter regular
	{
		char regular[2] = {str[*i], '\0'};                                                // Crea un string de un solo carácter
		expanded = ft_strdup(regular);                                                    // Duplica el carácter
		(*i)++;                                                                           // Avanza el índice
	}
	return (expanded);                                                                    // Devuelve el valor expandido o el carácter
}

/*
** Expande la cadena completa procesando cada parte y reemplazando variables de entorno.
** Concatena las expansiones y devuelve el resultado final.
*/
static char	*expand_string(const char *str, t_minishell *shell)
{
	char	*result;
	char	*temp;
	char	*expanded;
	int		i;

	result = ft_strdup("");                                                               // Inicializa un string vacío para el resultado
	i = 0;                                                                                // Comienza a procesar desde el inicio de la cadena
	while (str[i] != '\0')                                                                // Recorre la cadena completa
	{
		expanded = expand_var_or_char(str, &i, shell);                                    // Expande cada variable o carácter
		temp = ft_strjoin(result, expanded);                                              // Concatena al resultado acumulado
		free(result);                                                                     // Libera la cadena anterior
		free(expanded);                                                                   // Libera el valor expandido temporal
		result = temp;                                                                    // Actualiza el resultado con la nueva cadena
	}
	return (result);                                                                      // Devuelve la cadena completa expandida
}

/*
** Expande un argumento individual, si está entre comillas simples, lo deja intacto.
** En caso contrario, expande las variables de entorno contenidas en él.
*/
static char	*expand_argument(const char *arg, t_minishell *shell)
{
	if (ft_strlen(arg) >= 2 && arg[0] == '\'' && arg[ft_strlen(arg) - 1] == '\'')          // Verifica si el argumento está entre comillas simples
	{
		return (ft_strdup(arg));                                                          // Devuelve el argumento sin expandir
	}
	return (expand_string(arg, shell));                                                   // Expande el argumento y devuelve el resultado
}

/*
** Expande todos los tokens de la lista de argumentos de un comando.
** Si el argumento está entre comillas simples, no se expande.
** Los argumentos expandidos reemplazan a los originales.
*/
void	expand_tokens(t_cmd *cmd, t_minishell *shell)
{
	int		i;
	char	*expanded;

	i = 0;                                                                                // Inicializa el índice
	while (cmd->arguments && cmd->arguments[i] != NULL)                                   // Recorre todos los argumentos del comando
	{
		if (ft_strlen(cmd->arguments[i]) >= 2 &&                                          // Verifica si el argumento está entre comillas simples
			cmd->arguments[i][0] == '\'' && 
			cmd->arguments[i][ft_strlen(cmd->arguments[i]) - 1] == '\'')
		{
			// Argumento literal, no se expande
		}
		else                                                                              // Si no está entre comillas simples, se expande
		{
			expanded = expand_argument(cmd->arguments[i], shell);                         // Expande el argumento
			free(cmd->arguments[i]);                                                      // Libera el argumento original
			cmd->arguments[i] = expanded;                                                 // Reemplaza con el argumento expandido
		}
		i++;                                                                              // Avanza al siguiente argumento
	}
}