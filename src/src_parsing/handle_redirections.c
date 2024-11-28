/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_redirections.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albelope <albelope@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/16 11:57:47 by albelope          #+#    #+#             */
/*   Updated: 2024/11/25 13:18:12 by albelope         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int get_redirection_type(char *token)
{																			
	if (!token)																												// Si el token es NULL, no es una redirección válida
		return (NOT_REDIR);																									// Retorna -1 para indicar redirección no válida
	if (ft_strncmp(token, ">", 1) == 0 && ft_strlen(token) == 1)															// Verifica si el token es ">" (salida)
		return (OUTFILE);																									// Retorna tipo OUTFILE
	else if (ft_strncmp(token, ">>", 2) == 0 && ft_strlen(token) == 2)														// Verifica si el token es ">>" (append)
		return (APPEND);																									// Retorna tipo APPEND
	else if (ft_strncmp(token, "<", 1) == 0 && ft_strlen(token) == 1)														// Verifica si el token es "<" (entrada)
		return (INFILE);																									// Retorna tipo INFILE
	else if (ft_strncmp(token, "<<", 2) == 0 && ft_strlen(token) == 2)														// Verifica si el token es "<<" (here-document)
		return (HEREDOC);																									// Retorna tipo HEREDOC
	return (NOT_REDIR);																										// Si no coincide con ninguna, retorna -1
}

int create_and_add_redirection(t_cmd *cmd, int type, char *file)
{
    t_redir *new_redir; 																			// Puntero para la nueva redirección
    t_redir *current;   																			// Puntero para recorrer la lista de redirecciones existentes
																							
    new_redir = malloc(sizeof(t_redir));
    if (!new_redir)
    {
        perror("[ERROR] create_and_add_redirection: Memory allocation failed");
        return (-1); 																				// Retorna -1 si falla la asignación
    }																									 // Inicializar la nueva redirección
    new_redir->type = type;
    new_redir->file = ft_strdup(file);
    new_redir->next = NULL;
    if (!new_redir->file)
    {
        free(new_redir); 																			// Liberar memoria si la duplicación falla
        perror("[ERROR] create_and_add_redirection: File duplication failed");
        return (-1);
    }  																								// Añadir la nueva redirección a la lista de redirecciones del comando
    if (!cmd->redir) 																				// Si no hay redirecciones previas
    {
        cmd->redir = new_redir;
    }
    else
    {
        current = cmd->redir;
        while (current->next) 																		// Avanzar al final de la lista
            current = current->next;
        current->next = new_redir; 																	// Añadir al final de la lista
    }																			
	return (0);
}

int validate_redirection_syntax(char **tokens, int *i)
{
    int next_type;

    if (tokens[*i + 1] != NULL)
        next_type = get_redirection_type(tokens[*i + 1]);
    else
        next_type = NOT_REDIR;
    if (next_type != NOT_REDIR)
        return (-1);
    if (tokens[*i + 1] == NULL || is_empty_or_whitespace(tokens[*i + 1]))
        return (-1);
    return (0);
}

int process_redirection(char **tokens, int *i, t_cmd *cmd, t_minishell *shell)
{
	int		type;																												// Tipo de redirección
	char	*expanded_filename;																								// Nombre del archivo expandido

	if (tokens[*i] == NULL || is_empty_or_whitespace(tokens[*i]))															// Verifica si el token actual es NULL o vacío
		return (-1);																										// Retorna -1 en caso de error
	type = get_redirection_type(tokens[*i]);																				// Obtiene el tipo de redirección
	if (type == NOT_REDIR)																									// Si no es una redirección
		return (1);	
	if (validate_redirection_syntax(tokens, i) == -1)																		// Valida la sintaxis de la redirección
	{
		(*i)++;																												// Incrementa el índice
		return (-1);																										// Retorna -1 en caso de error
	}
	if (type == HEREDOC)																									// Si el tipo de redirección es HEREDOC
		return (parse_heredoc(shell, tokens, i, cmd));
	expanded_filename = expand_string(tokens[*i + 1], shell);																// Expande el nombre del archivo
	if (!expanded_filename || is_empty_or_whitespace(expanded_filename))													// Verifica si la expansión falló
		return (-1);																										// Retorna -1 en caso de error
	if (create_and_add_redirection(cmd, type, expanded_filename) == -1)														// Crea y añade la redirección
	{
		free(expanded_filename);																							// Libera el nombre del archivo expandido
		return (-1);																										// Retorna -1 en caso de error
	}
	free(expanded_filename);																								// Libera el nombre del archivo expandido
	*i += 2;																												// Incrementa el índice en 2 para pasar al siguiente token
	return (0);																												// Retorna 0 si todo ha sido correctamente procesado
}

