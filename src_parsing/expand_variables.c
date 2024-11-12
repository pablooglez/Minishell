/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_variables.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albelope <albelope@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/17 22:02:04 by albelope          #+#    #+#             */
/*   Updated: 2024/11/12 20:36:18 by albelope         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char *find_env_value(t_env *env_vars, const char *key)
{
	while (env_vars)														// Recorre la lista de variables de entorno
	{
		if (ft_strncmp(env_vars->key, key, ft_strlen(env_vars->key)) == 0)	// Compara el key de la variable actual
			return (env_vars->value);										// Retorna el valor si la clave coincide
		env_vars = env_vars->next;											// Pasa a la siguiente variable en la lista
	}
	return (NULL);															// Retorna NULL si no encuentra la clave
}

char *get_expanded_value(const char *variable, t_minishell *shell)
{
	char *value;

	if (ft_strncmp(variable, "?", 2) == 0)									// Verifica si la variable es "?"
		return (ft_itoa(shell->exit_status));								// Retorna el valor del estado de salida como string

	value = find_env_value(shell->env_vars, variable);						// Busca el valor de la variable en env_vars
	if (value)
		return (ft_strdup(value));											// Retorna una copia del valor si existe
	return (ft_strdup(""));													// Retorna una cadena vacía si no existe
}

char *expand_var_or_char(const char *str, int *i, t_minishell *shell)
{
	char *expanded;

	if (str[*i] == '\\' && str[*i + 1] == '$')								// Verifica si hay un dólar escapado
		expanded = handle_escaped_dollar(i);								// Maneja el carácter '$' escapado
	else if (str[*i] == '$')												// Verifica si es un símbolo de dólar
		expanded = handle_dollar_sign(str, i, shell);						// Expande la variable después de '$'
	else
		expanded = handle_regular_char(str, i);								// Trata cualquier otro carácter
	return (expanded);														// Retorna el carácter expandido
}

char *expand_string(const char *str, t_minishell *shell)
{
	char	*result;
	char	*expanded;
	char	*temp;
	int		i;

	result = ft_strdup("");													// Inicializa result con una cadena vacía
	if (!result)
		return (NULL);														// Retorna NULL si falla la asignación
	i = 0;
	while (str[i] != '\0')													// Recorre cada carácter de la cadena
	{
		expanded = expand_var_or_char(str, &i, shell);						// Expande el carácter actual o variable
		if (!expanded)
		{
			free(result);													// Libera result si falla la expansión
			return (NULL);
		}

		temp = ft_strjoin(result, expanded);								// Concatena el resultado actual con el nuevo expandido
		free(result);
		free(expanded);
		if (!temp)
			return (NULL);													// Retorna NULL si falla la concatenación
		result = temp;														// Actualiza result con la concatenación
	}
	return (result);														// Retorna la cadena expandida completa
}

char *expand_entire_input(const char *input, t_minishell *shell)
{
	char *expanded;

	expanded = expand_string(input, shell);									// Expande toda la cadena input
	printf("[DEBUG]-->EXPAND_ENTIRE_INPUT==> Cadena expandida:            [%s]\n", expanded);
	if (expanded != NULL && access(expanded, X_OK) == 0)					// Verifica si la expansión es ejecutable
		return (expanded);													// Retorna la cadena expandida si es ejecutable
	free(expanded);															// Libera expanded si no es ejecutable
	return (NULL);															// Retorna NULL si no se puede ejecutar
}

void expand_tokens(t_cmd *cmd, t_minishell *shell)
{
	int		i;
	char	*expanded;

	i = 0;
	while (cmd->arguments && cmd->arguments[i] != NULL)						// Recorre cada argumento en cmd
	{
		printf("[DEBUG]-->EXPAND_TOKENS 0. 1 ==> Argumento antes de expandir:      [%s]\n", cmd->arguments[i]);
		expanded = expand_argument(cmd->arguments[i], shell);				// Expande el argumento actual
		free(cmd->arguments[i]);											// Libera el argumento original
		cmd->arguments[i] = expanded;										// Actualiza el argumento con la expansión
		i++;
	}
}
