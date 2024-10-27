/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_variables.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albelope <albelope@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/17 22:02:04 by albelope          #+#    #+#             */
/*   Updated: 2024/10/27 13:59:44 by albelope         ###   ########.fr       */
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
/*static char	*expand_var_or_char(const char *str, int *i, t_minishell *shell)
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
}*/
/*static char *expand_var_or_char(const char *str, int *i, t_minishell *shell)
{
    char *expanded;
    int start;

    if (str[*i] == '$')
    {
        if (str[*i + 1] == '?')
        {
            expanded = get_expanded_value("?", shell->exit_status);
            *i += 2;
        }
        else if (ft_isalpha(str[*i + 1]) || str[*i + 1] == '_')
        {
            start = *i + 1;
            while (str[start] && (ft_isalnum(str[start]) || str[start] == '_'))
                start++;
            char *var_name = ft_substr(str, *i + 1, start - (*i + 1));
            expanded = get_expanded_value(var_name, shell->exit_status);
            free(var_name);
            *i = start;
        }
        else
        {
            expanded = ft_strdup("$");
            (*i)++;
        }
    }
    else
    {
        expanded = ft_substr(str, *i, 1);
        (*i)++;
    }
    return (expanded);
}*/
#include "../include/minishell.h"

/*
** Expande la variable de entorno o devuelve el carácter literal si no corresponde a una expansión válida.
** Soporta caracteres de escape `\$` para devolver `$` literal y expande `$?` al estado de salida.
*/
static char	*expand_var_or_char(const char *str, int *i, t_minishell *shell)
{
	char	*expanded;
	char	*var_name;
	int		start;

	if (str[*i] == '\\' && str[*i + 1] == '$')
	{
		expanded = ft_strdup("$");
		*i += 2;
	}
	else if (str[*i] == '$')
	{
		if (str[*i + 1] == '?')
		{
			expanded = get_expanded_value("?", shell->exit_status);
			*i += 2;
		}
		else if (ft_isalpha(str[*i + 1]) || str[*i + 1] == '_')
		{
			start = *i + 1;
			while (str[start] && (ft_isalnum(str[start]) || str[start] == '_'))
				start++;
			var_name = ft_substr(str, *i + 1, start - (*i + 1));
			expanded = get_expanded_value(var_name, shell->exit_status);
			free(var_name);
			*i = start;
		}
		else
		{
			expanded = ft_strdup("$");
			(*i)++;
		}
	}
	else
	{
		expanded = ft_substr(str, *i, 1);
		(*i)++;
	}
	return (expanded);
}



/*
** Expande la cadena completa procesando cada parte y reemplazando variables de entorno.
** Concatena las expansiones y devuelve el resultado final.
*/
static char *expand_string(const char *str, t_minishell *shell)
{
    char *result;
    char *temp;
    char *expanded;
    int i;

    result = ft_strdup("");
    if (!result)
        return (NULL);
    i = 0;
    while (str[i] != '\0')
    {
        expanded = expand_var_or_char(str, &i, shell);
        if (!expanded)
        {
            free(result);
            return (NULL);
        }
        temp = ft_strjoin(result, expanded);
        free(result);
        free(expanded);
        if (!temp)
            return (NULL);
        result = temp;
    }
    return (result);
}


/*
** Expande un argumento individual, si está entre comillas simples, lo deja intacto.
** En caso contrario, expande las variables de entorno contenidas en él.
*/
static char *remove_quotes(const char *arg)
{
    size_t len;

    len = ft_strlen(arg);
    if ((arg[0] == '\'' && arg[len - 1] == '\'') ||
        (arg[0] == '\"' && arg[len - 1] == '\"'))
        return (ft_substr(arg, 1, len - 2));
    return (ft_strdup(arg));
}

static char *expand_argument(const char *arg, t_minishell *shell)
{
    char *unquoted;
    char *expanded;

    if ((arg[0] == '\'' && arg[ft_strlen(arg) - 1] == '\''))
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
    {
        return (expand_string(arg, shell));
    }
}


/*
** Expande todos los tokens de la lista de argumentos de un comando.
** Si el argumento está entre comillas simples, no se expande.
** Los argumentos expandidos reemplazan a los originales.
*/
void expand_tokens(t_cmd *cmd, t_minishell *shell)
{
    int i;
    char *expanded;

    i = 0;
    while (cmd->arguments && cmd->arguments[i] != NULL)
    {
        expanded = expand_argument(cmd->arguments[i], shell);
        free(cmd->arguments[i]);
        cmd->arguments[i] = expanded;
        i++;
    }
}
