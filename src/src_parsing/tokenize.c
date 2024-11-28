/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albelope <albelope@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/11 20:39:51 by albelope          #+#    #+#             */
/*   Updated: 2024/11/25 13:23:38 by albelope         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


t_token	classify_special_token(char c)
{
	if (c == '|')																			// Verifica si el carcates es un Pipe
		return (PIPE);																		// Retorna PIPE si el carácter es un Pipe
	if (c == '>')																			// Verifica si el Caracter es una Redireccion
		return (REDIR);
	if (c == '<')
		return (REDIR);
	return (UNKNOWN);
}

int process_tokens(char **tokens, t_cmd *current_cmd, t_minishell *shell)
{
    int i = 0;

    while (tokens[i])
    {
        int redir_type = get_redirection_type(tokens[i]);
        
        if (redir_type != NOT_REDIR)
        {
            if (process_redirection(tokens, &i, current_cmd, shell) == -1)
                return (-1);
        }
        else if (ft_strncmp(tokens[i], "|", 1) == 0)
        {
            if (process_token_pipe(tokens, &i, &current_cmd, shell) == -1)
                return (-1);
        }
        else
        {
            if (process_arguments(tokens, &i, current_cmd, shell) == -1)
                return (-1);
        }

        if (tokens[i] && current_cmd->type == PIPE)
        {
            if (!current_cmd->next)
                return (-1);
            current_cmd = current_cmd->next;
        }
    }
    return (0);
}


int	handle_escape(char *input, int i, char *buffer, int *buf_index)
{

	/*i++;																	// Incrementa el índice para pasar al siguiente carácter
	if (input[i] == '"' || input[i] == '\'' || input[i] == '\\')			// Verifica si el carácter es una comilla doble, comilla simple o barra invertida
		buffer[(*buf_index)++] = input[i++];								// Añade el carácter al buffer y aumenta el índice
	else												
	{
		buffer[(*buf_index)++] = '\\';										// Añade una barra invertida al buffer
		buffer[(*buf_index)++] = input[i++];								// Añade el carácter al buffer	
	}
	return (i);*/																// Retorna el índice actualizado
	buffer[(*buf_index)++] = input[i++];
    if (input[i]) 
	{ 
        buffer[(*buf_index)++] = input[i++];
    }
    return (i);
}

int	expand_variable(char *input, char *buffer, int *buf_index, t_minishell *shell)
{
	char	var_name[256];
	int		var_len;
	char	*pid;
	int		k;
	char	*value;

	var_len = 0;
	shell->i++; 
	if (input[shell->i] == '\0' || input[shell->i] == ' ')
	{
		buffer[(*buf_index)++] = '$';
		return (shell->i);
	}
	if (input[shell->i] == '$')
	{
		shell->i++;
		pid = ft_itoa(getpid());
		k = 0;
		while (pid[k])
			buffer[(*buf_index)++] = pid[k++];
		free(pid);
		return (shell->i);
	}
	while (input[shell->i] && (ft_isalnum(input[shell->i]) || input[shell->i] == '_' || input[shell->i] == '?'))
		var_name[var_len++] = input[shell->i++];
	var_name[var_len] = '\0';                                                   //  ********** KOBAYASHI **********
    if (ft_strncmp(var_name, "?", 1) == 0)                                      //  Esto trata $?, pero creo que get_env_value devuelve un puntero, pero itoa hace malloc ademas, por lo
		value = ft_itoa(shell->exit_status);                                    //  tanto, es probable que haya un leak si no se trata. La cosa es como determinar cuando liberar value, porque
	else                                                                        //  no se deberia liberar si se usa con get_env_value, pero si cuando es itoa.
        value = ft_strdup(get_env_value(shell->env_vars, var_name));            //  Al final haciendo un strdup, abajo se libera value y todos contentos.
	if (value)
	{
		k = 0;
		while (value[k])
			buffer[(*buf_index)++] = value[k++];
        free(value);                                                            //  Liberamos value
	}
	return (shell->i);
}


int	handle_token(char *input, char **tokens, int *j, t_minishell *shell)
{
	char	buffer[1024];
	int		buf_index = 0;
	int		no_expand = 0;

	while (input[shell->i] && input[shell->i] != ' ')
	{
		if (input[shell->i] == '\'')
		{
			no_expand = 1;
			shell->i++; 
			while (input[shell->i] && input[shell->i] != '\'')
				buffer[buf_index++] = input[shell->i++];
			if (input[shell->i] != '\'')
				return (-1);
			shell->i++;
		}
		else if (input[shell->i] == '"')
		{
			shell->i++;
			while (input[shell->i] && input[shell->i] != '"')
			{
				if (input[shell->i] == '$') 
				{
					shell->i = expand_variable(input, buffer, &buf_index, shell);
					if (shell->i == -1)
						return (-1);
				}
				else
					buffer[buf_index++] = input[shell->i++];
			}
			if (input[shell->i] != '"') 
				return (-1);
			shell->i++; 
		}
		else if (input[shell->i] == '$')
		{
			shell->i = expand_variable(input, buffer, &buf_index, shell);
			if (shell->i == -1)
				return (-1);
		}
		else if (classify_special_token(input[shell->i]) != UNKNOWN)
		{
			if (buf_index > 0)
			{
				buffer[buf_index] = '\0';
				tokens[*j] = ft_strdup(buffer);
				if (!tokens[*j])
				{
					free_tokens_parse(tokens);
					return (-1);
				}
				(*j)++;
				buf_index = 0;
			}
			tokens[*j] = ft_substr(input, shell->i, 1);
			if (!tokens[*j])
			{
				free_tokens_parse(tokens);
				return (-1);
			}
			(*j)++;
			shell->i++;
			return (shell->i);
		}
		else
			buffer[buf_index++] = input[shell->i++];
	}
	buffer[buf_index] = '\0';
	if (buf_index > 0)
	{ 
		if (no_expand)
			tokens[*j] = ft_strjoin("__NO_EXPAND__", buffer);
		else
			tokens[*j] = ft_strdup(buffer);
		if (!tokens[*j]) 
		{
			free_tokens_parse(tokens);
			return (-1);
		}
		(*j)++;
	}
	return (shell->i);
}


char	**tokenize_input(char *input, t_minishell *shell)
{
	char	**tokens;
	int		j;

	shell->i = 0;
	j = 0;
	tokens = ft_calloc(100, sizeof(char *));
	if (!tokens)
		return (NULL);
	while (input[shell->i])
	{
		while (input[shell->i] == ' ')
			shell->i++;
		if (process_token(input, tokens, &j, shell) == -1)
		{
			free_tokens_parse(tokens);
			return (NULL);
		}
	}
	tokens[j] = NULL;
	return (tokens);
}
