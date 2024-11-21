/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_redirections.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pabloglez <pabloglez@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/16 11:57:47 by albelope          #+#    #+#             */
/*   Updated: 2024/11/21 16:49:45 by pabloglez        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int get_redirection_type(char *token)
{
	//printf("[DEBUG] get_redirection_type: Token: %s\n", token);																// Imprime el token recibido
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

int parse_heredoc(char **tokens, int *i, t_cmd *cmd)
{
	t_redir *new_redir;																										// Puntero para la nueva redirección
	t_redir *current;																										// Puntero para recorrer la lista de redirecciones existentes

	new_redir = malloc(sizeof(t_redir));																					// Asigna memoria para la nueva redirección
	if (!new_redir)																											// Verifica si la asignación falló
		return (-1);																										// Retorna -1 en caso de error
	new_redir->type = HEREDOC;																								// Establece el tipo de redirección
	new_redir->file = ft_strdup(tokens[*i + 1]);																			// Duplica el delimitador
	if (!new_redir->file)																									// Verifica si la duplicación falló
	{
		free(new_redir);																									// Libera la memoria asignada a la redirección si hubo error
		return (-1);																										// Retorna -1 en caso de error
	}
	new_redir->next = NULL;																									// Establece el puntero `next` como NULL
	if (!cmd->redir)																										// Si no hay redirecciones previas en el comando
		cmd->redir = new_redir;																								// Asigna la nueva redirección como la primera
	else																													// Si ya existen redirecciones
	{
		current = cmd->redir;																								// Inicia en la primera redirección
		while (current->next)																								// Recorre la lista hasta el último elemento
			current = current->next;																						// Avanza al siguiente
		current->next = new_redir;																							// Añade la nueva redirección al final de la lista
	}
	*i += 1;																												// Incrementa el índice
	return (0);																												// Retorna 0 si todo ha sido correctamente procesado
}

/*int create_and_add_redirection(t_cmd *cmd, int type, char *file)
{
	t_redir *new_redir;																										// Puntero para la nueva redirección
	t_redir *current;																										// Puntero para recorrer la lista de redirecciones existentes

	//printf("[DEBUG] create_and_add_redirection: Adding redirection (type: %d, file: %s)\n", type, file);
	if (type == OUTFILE || type == APPEND || type == INFILE)																					// Verifica si el tipo es salida o append
	{
		current = cmd->redir;																								// Inicia en la primera redirección
		while (current)																										// Recorre la lista de redirecciones
		{
			if (current->type == type)																						// Verifica si existe una redirección del mismo tipo
			{
				free(current->file);																						// Libera el archivo existente
				current->file = ft_strdup(file);																			// Duplica el nuevo archivo
				//printf("[DEBUG] create_and_add_redirection: Updated redirection (type: %d, file: %s)\n", type, file);
				return (0);																									// Retorna 0 si se actualizó correctamente
			}
			current = current->next;																						// Avanza al siguiente nodo
		}
	}
	new_redir = malloc(sizeof(t_redir));																					// Asigna memoria para la nueva redirección
	if (!new_redir)																											// Verifica si la asignación falló
		return (-1);																										// Retorna -1 en caso de error
	new_redir->type = type;																									// Asigna el tipo de redirección
	new_redir->file = ft_strdup(file);																						// Duplica el nombre del archivo para la redirección
	if (!new_redir->file)																									// Verifica si la duplicación falló
	{
		free(new_redir);																									// Libera la memoria asignada a la redirección si hubo error
		return (-1);																										// Retorna -1 en caso de error
	}
	new_redir->next = NULL;																									// Establece el puntero `next` de la redirección como NULL
	if (!cmd->redir)																										// Si no hay redirecciones previas en el comando
		cmd->redir = new_redir;																								// Asigna la nueva redirección como la primera
	else																													// Si ya existen redirecciones
	{
		current = cmd->redir;																								// Inicia en la primera redirección
		while (current->next)																								// Recorre la lista hasta el último elemento
			current = current->next;																						// Avanza al siguiente
		current->next = new_redir;																							// Añade la nueva redirección al final de la lista
	}
	//printf("[DEBUG] create_and_add_redirection: Added redirection (type: %d, file: %s)\n", type, file);
	return (0);																												// Retorna 0 si todo ha sido correctamente procesado
}*/
int create_and_add_redirection(t_cmd *cmd, int type, char *file)
{
	//printf("[DEBUG] create_and_add_redirection: Adding redirection (type: %d, file: %s)\n", type, file);
    t_redir *new_redir; 																			// Puntero para la nueva redirección
    t_redir *current;   																			// Puntero para recorrer la lista de redirecciones existentes

    //printf("[DEBUG] create_and_add_redirection: Adding redirection (type: %d, file: %s)\n", type, file);

    																								// Crear una nueva redirección
    new_redir = malloc(sizeof(t_redir));
    if (!new_redir)
    {
        perror("[ERROR] create_and_add_redirection: Memory allocation failed");
        return (-1); 																				// Retorna -1 si falla la asignación
    }

   																									 // Inicializar la nueva redirección
    new_redir->type = type;
    new_redir->file = ft_strdup(file);
    new_redir->next = NULL;

    if (!new_redir->file)
    {
        free(new_redir); 																			// Liberar memoria si la duplicación falla
        perror("[ERROR] create_and_add_redirection: File duplication failed");
        return (-1);
    }

    																								// Añadir la nueva redirección a la lista de redirecciones del comando
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
	int	current_type;																										// Tipo de redirección actual
	int	next_type;																											// Tipo de redirección siguiente

	current_type = get_redirection_type(tokens[*i]);																		// Obtiene el tipo del token actual
	if (tokens[*i + 1] != NULL)																								// Verifica si hay un siguiente token
		next_type = get_redirection_type(tokens[*i + 1]);																	// Obtiene el tipo del siguiente token
	else
		next_type = NOT_REDIR;																								// Si no hay siguiente, asigna tipo no redirección
	if (next_type != NOT_REDIR)																								// Verifica si hay un error de redirección
		return (-1);																										// Retorna -1 indicando un error
	if (tokens[*i + 1] == NULL || is_empty_or_whitespace(tokens[*i + 1]))													// Verifica si falta el archivo
		return (-1);																										// Retorna -1 indicando un error
	if (*i == 0 && current_type != NOT_REDIR)																				// Verifica si el token actual es el primero y si es una redirección									
		return (-1);																										// Retorna -1 indicando un error
	return (0);																												// Retorna 0 indicando que la sintaxis es válida
}

int process_redirection(char **tokens, int *i, t_cmd *cmd, t_minishell *shell)
{
	int		type;																												// Tipo de redirección
	char	*expanded_filename;																								// Nombre del archivo expandido

	//printf("[DEBUG] process_redirection: Processing redirection token: %s\n", tokens[*i]);
	if (tokens[*i] == NULL || is_empty_or_whitespace(tokens[*i]))															// Verifica si el token actual es NULL o vacío
		return (-1);																										// Retorna -1 en caso de error
	type = get_redirection_type(tokens[*i]);																				// Obtiene el tipo de redirección
	if (type == NOT_REDIR)																									// Si no es una redirección
		return (1);																											// Retorna 1 para indicar que no se procesó redirección
	if (validate_redirection_syntax(tokens, i) == -1)																		// Valida la sintaxis de la redirección
	{
		//printf("[ERROR] process_redirection: Invalid redirection syntax: %s\n", tokens[*i]);
		(*i)++;																												// Incrementa el índice
		return (-1);																										// Retorna -1 en caso de error
	}
	if (type == HEREDOC)																									// Si el tipo de redirección es HEREDOC
	{
		if (parse_heredoc(tokens, i, cmd) == -1)																			// Procesa la redirección HEREDOC
			return (-1);																									// Retorna -1 en caso de error
		(*i)++;																												// Incrementa el índice
		return (0);																											// Retorna 0 si se procesó correctamente
	}
	//printf("[DEBUG] process_redirection: Processing redirection type: %d\n", type);
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

