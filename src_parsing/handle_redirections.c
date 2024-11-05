/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_redirections.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pabloglez <pabloglez@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/16 11:57:47 by albelope          #+#    #+#             */
/*   Updated: 2024/11/04 20:50:23 by pabloglez        ###   ########.fr       */
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

int parse_heredoc(char **tokens, int *i, t_cmd *cmd)
{
	t_redir *new_redir;																										// Puntero para la nueva redirección
	t_redir *current;																										// Puntero para recorrer la lista de redirecciones existentes

	printf("(DEBUG) parse_heredoc: Token actual es '%s'\n", tokens[*i]);													// Imprime el token actual
	printf("(DEBUG) parse_heredoc: Delimitador heredoc es '%s'\n", tokens[*i + 1]);											// Imprime el delimitador heredoc
	new_redir = malloc(sizeof(t_redir));																					// Asigna memoria para la nueva redirección
	if (!new_redir)																											// Verifica si la asignación falló
		return (-1);																										// Retorna -1 en caso de error
	new_redir->type = HEREDOC;																								// Establece el tipo de redirección
	new_redir->file = ft_strdup(tokens[*i + 1]);																			// Duplica el delimitador
	if (!new_redir->file)																									// Verifica si la duplicación falló
	{
		free(new_redir);																									// Libera la memoria asignada a la redirección si hubo error
		printf("(ERROR) parse_heredoc: Error al duplicar el delimitador\n");												// Imprime un mensaje de error
		return (-1);																										// Retorna -1 en caso de error
	}
	printf("(DEBUG) parse_heredoc: Redirección heredoc agregada con delimitador '%s'\n", new_redir->file);					// Mensaje de confirmación
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

int create_and_add_redirection(t_cmd *cmd, int type, char *file)
{
	t_redir *new_redir;																										// Puntero para la nueva redirección
	t_redir *current;																										// Puntero para recorrer la lista de redirecciones existentes

	if (type == OUTFILE || type == APPEND)																					// Verifica si el tipo es salida o append
	{
		current = cmd->redir;																								// Inicia en la primera redirección
		while (current)																										// Recorre la lista de redirecciones
		{
			if (current->type == type)																						// Verifica si existe una redirección del mismo tipo
			{
				free(current->file);																						// Libera el archivo existente
				current->file = ft_strdup(file);																			// Duplica el nuevo archivo
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
	return (0);																												// Retorna 0 si todo ha sido correctamente procesado
}

int is_empty_or_whitespace(char *str)
{
	if (!str)																												// Verifica si el string es NULL
		return (1);																											// Retorna 1 si está vacío
	while (*str)																											// Recorre cada carácter del string
	{
		if (*str != ' ')																									// Si encuentra un carácter diferente de espacio
			return (0);																										// Retorna 0 indicando que no está vacío
		str++;																												// Avanza al siguiente carácter
	}
	return (1);																												// Retorna 1 si todos los caracteres son espacios
}

int validate_redirection_syntax(char **tokens, int *i)
{
	int	current_type;																										// Tipo de redirección actual
	int	next_type;																											// Tipo de redirección siguiente

	current_type = get_redirection_type(tokens[*i]);																		// Obtiene el tipo del token actual
	printf("(DEBUG) validate_redirection_syntax: Token actual '%s', Tipo %d\n", tokens[*i], current_type);					// Imprime para depuración
	if (tokens[*i + 1] != NULL)																								// Verifica si hay un siguiente token
		next_type = get_redirection_type(tokens[*i + 1]);																	// Obtiene el tipo del siguiente token
	else
		next_type = NOT_REDIR;																								// Si no hay siguiente, asigna tipo no redirección
	if (next_type != NOT_REDIR)																								// Verifica si hay un error de redirección
	{
		printf("Error: redirección duplicada o mal formada.\n");															// Imprime un mensaje de error
		return (-1);																										// Retorna -1 indicando un error
	}
	if (tokens[*i + 1] == NULL || is_empty_or_whitespace(tokens[*i + 1]))													// Verifica si falta el archivo
	{
		printf("Error: falta archivo o delimitador para la redirección.\n");												// Imprime un mensaje de error
		return (-1);																										// Retorna -1 indicando un error
	}
	if (*i == 0 && current_type != NOT_REDIR)																				// Verifica si el token actual es el primero y si es una redirección
	{
		printf("Error: falta comando antes de la redirección.\n");															// Imprime un mensaje de error
		return (-1);																										// Retorna -1 indicando un error
	}
	printf("(DEBUG) validate_redirection_syntax: Sintaxis de redirección validada correctamente\n");						// Imprime mensaje de éxito
	return (0);																												// Retorna 0 indicando que la sintaxis es válida
}

int process_redirection(char **tokens, int *i, t_cmd *cmd, t_minishell *shell)
{
	int		type;																												// Tipo de redirección
	char	*expanded_filename;																								// Nombre del archivo expandido

	if (tokens[*i] == NULL || is_empty_or_whitespace(tokens[*i]))															// Verifica si el token actual es NULL o vacío
		return (-1);																										// Retorna -1 en caso de error
	type = get_redirection_type(tokens[*i]);																				// Obtiene el tipo de redirección
	printf("(DEBUG) process_redirection: Procesando redirección tipo %d para token '%s'\n", type, tokens[*i]); 				// Imprime para depuración
	if (type == NOT_REDIR)																									// Si no es una redirección
		return (1);																											// Retorna 1 para indicar que no se procesó redirección
	if (validate_redirection_syntax(tokens, i) == -1)																		// Valida la sintaxis de la redirección
	{
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
	expanded_filename = expand_string(tokens[*i + 1], shell);																// Expande el nombre del archivo
	if (!expanded_filename || is_empty_or_whitespace(expanded_filename))													// Verifica si la expansión falló
	{
		printf("Error: expansión fallida en el nombre del archivo.\n");														// Imprime un mensaje de error
		return (-1);																										// Retorna -1 en caso de error
	}
	if (create_and_add_redirection(cmd, type, expanded_filename) == -1)														// Crea y añade la redirección
	{
		free(expanded_filename);																							// Libera el nombre del archivo expandido
		return (-1);																										// Retorna -1 en caso de error
	}
	printf("(DEBUG) process_redirection: Redirección agregada para archivo '%s' con tipo %d\n", expanded_filename, type);	// Mensaje de éxito
	free(expanded_filename);																								// Libera el nombre del archivo expandido
	*i += 2;																												// Incrementa el índice en 2 para pasar al siguiente token
	return (0);																												// Retorna 0 si todo ha sido correctamente procesado
}

void free_redirections(t_redir *redir)
{
	t_redir *temp;																											// Puntero temporal para liberar cada redirección

	while (redir)																											// Recorre toda la lista de redirecciones
	{
		temp = redir;																										// Guarda la redirección actual en `temp`
		redir = redir->next;																								// Avanza a la siguiente redirección
		free(temp->file);																									// Libera la memoria del nombre del archivo de la redirección
		free(temp);																											// Libera la estructura de la redirección
	}
}
