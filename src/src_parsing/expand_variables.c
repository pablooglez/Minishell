/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_variables.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albelope <albelope@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/17 22:02:04 by albelope          #+#    #+#             */
/*   Updated: 2024/11/29 22:45:40 by albelope         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*find_env_value(t_env *env_vars, const char *key)
{
	size_t	key_len;
	size_t	current_key_len;

	if (!key || !env_vars)
		return (NULL);
	key_len = ft_strlen(key);
	while (env_vars != NULL)
	{
		current_key_len = ft_strlen(env_vars->key);
		if (current_key_len == key_len)
		{
			if (ft_strncmp(env_vars->key, key, key_len) == 0)
				return (env_vars->value);
		}
		env_vars = env_vars->next;
	}
	return (NULL);
}

char	*get_expanded_value(const char *variable, t_minishell *shell)
{
	char	*value;

	if (ft_strncmp(variable, "?", 2) == 0)
		return (ft_itoa(shell->exit_status));
	value = find_env_value(shell->env_vars, variable);
	if (value)
		return (ft_strdup(value));
	return (ft_strdup(""));
}

char	*expand_var_or_char(const char *str, int *i, t_minishell *shell)
{
	char	*expanded;

	if (str[*i] == '\\' && str[*i + 1] == '$')
		expanded = handle_escaped_dollar(i);
	else if (str[*i] == '$')
		expanded = handle_dollar_sign(str, i, shell);
	else
		expanded = handle_regular_char(str, i);
	return (expanded);
}

char	*expand_string(const char *str, t_minishell *shell)
{
	char	*result;
	char	*expanded;
	char	*temp;
	int		i;

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
