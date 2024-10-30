/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_redirections.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albelope <albelope@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/16 11:57:47 by albelope          #+#    #+#             */
/*   Updated: 2024/10/30 21:32:53 by albelope         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	get_redirection_type(char *token)
{
	if (!token)                                                                              // Si el token es NULL, no es una redirección válida
		return (NOT_REDIR);                                                                         // Retorna -1 para indicar redirección no válida
	if (ft_strncmp(token, ">", 1) == 0 && ft_strlen(token) == 1)                             // Verifica si el token es ">" (salida)
		return (OUTFILE);                                                                    // Retorna tipo OUTFILE
	else if (ft_strncmp(token, ">>", 2) == 0 && ft_strlen(token) == 2)                       // Verifica si el token es ">>" (append)
		return (APPEND);                                                                     // Retorna tipo APPEND
	else if (ft_strncmp(token, "<", 1) == 0 && ft_strlen(token) == 1)                        // Verifica si el token es "<" (entrada)
		return (INFILE);                                                                     // Retorna tipo INFILE
	else if (ft_strncmp(token, "<<", 2) == 0 && ft_strlen(token) == 2)                       // Verifica si el token es "<<" (here-document)
		return (HEREDOC);                                                                    // Retorna tipo HEREDOC
	return (NOT_REDIR);                                                                             // Si no coincide con ninguna, retorna -1
}

int	parse_heredoc(char **tokens, int *i, t_cmd *cmd)
{
	t_redir	*new_redir;
	t_redir	*current;

	new_redir = malloc(sizeof(t_redir));
	if (!new_redir)
		return (-1);
	new_redir->type = HEREDOC;
	new_redir->file = ft_strdup(tokens[*i + 1]);
	if (!new_redir->file)
	{ 
		free(new_redir);
		return (-1);
	}
	new_redir->next = NULL;
	if (!cmd->redir)
		cmd->redir = new_redir;
	else
	{
		current = cmd->redir;
		while (current->next)
			current = current->next;
		current->next = new_redir;
	}
	*i += 1;
	return (0);
}

int	create_and_add_redirection(t_cmd *cmd, int type, char *file)
{
	t_redir	*new_redir;                                                                      // Puntero para la nueva redirección
	t_redir	*current;                                                                       // Puntero para recorrer la lista de redirecciones existentes

	if (type == OUTFILE || type == APPEND)
	{
		current = cmd->redir;
		while (current)
		{
			if (current->type == type)
			{
				free(current->file);
				current->file = ft_strdup(file);
				return (0);
			}
			current = current->next;
		}
	}
	new_redir = malloc(sizeof(t_redir));                                                     // Asigna memoria para la nueva redirección
	if (!new_redir)                                                                          // Verifica si la asignación falló
		return (-1);                                                                         // Retorna -1 en caso de error
	new_redir->type = type;                                                                  // Asigna el tipo de redirección
	new_redir->file = ft_strdup(file);                                                       // Duplica el nombre del archivo para la redirección
	if (!new_redir->file)                                                                    // Verifica si la duplicación falló
	{
		free(new_redir);                                                                     // Libera la memoria asignada a la redirección si hubo error
		return (-1);                                                                         // Retorna -1 en caso de error
	}
	new_redir->next = NULL;                                                                  // Establece el puntero `next` de la redirección como NULL
	if (!cmd->redir)                                                                         // Si no hay redirecciones previas en el comando
		cmd->redir = new_redir;                                                              // Asigna la nueva redirección como la primera
	else                                                                                     // Si ya existen redirecciones
	{
		current = cmd->redir;                                                                // Inicia en la primera redirección
		while (current->next)                                                                // Recorre la lista hasta el último elemento
			current = current->next;
		current->next = new_redir;                                                           // Añade la nueva redirección al final de la lista
	}
	return (0);                                                                              // Retorna 0 si todo ha sido correctamente procesado
}

int	is_empty_or_whitespace(char *str)
{
	if (!str)
		return (1);
	while (*str)
	{
		if (*str != ' ')
			return (0);
		str++;
	}
	return (1);
}
int validate_redirection_syntax(char **tokens, int *i)
{
    int current_type;
    int next_type;

    current_type = get_redirection_type(tokens[*i]);
    if (tokens[*i + 1] != NULL)
        next_type = get_redirection_type(tokens[*i + 1]);
    else
        next_type = NOT_REDIR;
    if (next_type != NOT_REDIR)
	{
        printf("Error: redirección duplicada o mal formada.\n");
        return (-1);
    }
	if (tokens[*i + 1] == NULL || is_empty_or_whitespace(tokens[*i + 1]))
	{
		printf("Error: falta archivo o delimitador para la redirección.\n");
		return (-1);
	}
    if (*i == 0 && current_type != NOT_REDIR)
	{
        printf("Error: falta comando antes de la redirección.\n");
        return (-1);
    }
    return (0);
}

int process_redirection(char **tokens, int *i, t_cmd *cmd, t_minishell *shell)
{
    int		type;
    char	*expanded_filename;

	if (tokens[*i] == NULL || is_empty_or_whitespace(tokens[*i]))
        return (-1);
    type = get_redirection_type(tokens[*i]);
    if (type == NOT_REDIR)
        return (1);
    if (validate_redirection_syntax(tokens, i) == -1)
	{
        (*i)++;
        return (-1);
    }
    if (type == HEREDOC)
	{
        if (parse_heredoc(tokens, i, cmd) == -1)
            return (-1);
        (*i)++;
        return (0);
    }
    expanded_filename = expand_string(tokens[*i + 1], shell);
    if (!expanded_filename)
	{
        printf("Error: expansión fallida en el nombre del archivo.\n");
        return (-1);
    }
    if (create_and_add_redirection(cmd, type, expanded_filename) == -1)
	{
        free(expanded_filename);
        return (-1);
    }
    free(expanded_filename);
    //(*i)++;
	*i += 2;
    return (0);
}

void	free_redirections(t_redir *redir)
{
	t_redir	*temp;                                                                          // Puntero temporal para liberar cada redirección

	while (redir)                                                                           // Recorre toda la lista de redirecciones
	{
		temp = redir;                                                                       // Guarda la redirección actual en `temp`
		redir = redir->next;                                                                // Avanza a la siguiente redirección
		free(temp->file);                                                                  // Libera la memoria del nombre del archivo de la redirección
		free(temp);                                                                        // Libera la estructura de la redirección
	}
}