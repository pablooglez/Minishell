/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_redirections.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albelope <albelope@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/16 11:57:47 by albelope          #+#    #+#             */
/*   Updated: 2024/10/25 13:17:06 by albelope         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

/* La función se encarga de identificar qué tipo de redirección es, es decir, si
   es una redirección de salida (>), de salida en modo append (>>), de entrada (<),
   o un here-document (<<). Si no es ninguna de estas, devuelve -1, indicando
   que no es una redirección válida. */
int	get_redirection_type(char *token)
{
	if (!token)                                                                              // Si el token es NULL, no es una redirección válida
		return (-1);                                                                         // Retorna -1 para indicar redirección no válida
	if (ft_strncmp(token, ">", 1) == 0 && ft_strlen(token) == 1)                             // Verifica si el token es ">" (salida)
		return (OUTFILE);                                                                    // Retorna tipo OUTFILE
	else if (ft_strncmp(token, ">>", 2) == 0 && ft_strlen(token) == 2)                       // Verifica si el token es ">>" (append)
		return (APPEND);                                                                     // Retorna tipo APPEND
	else if (ft_strncmp(token, "<", 1) == 0 && ft_strlen(token) == 1)                        // Verifica si el token es "<" (entrada)
		return (INFILE);                                                                     // Retorna tipo INFILE
	else if (ft_strncmp(token, "<<", 2) == 0 && ft_strlen(token) == 2)                       // Verifica si el token es "<<" (here-document)
		return (HEREDOC);                                                                    // Retorna tipo HEREDOC
	return (-1);                                                                             // Si no coincide con ninguna, retorna -1
}

/*
Crea una nueva redirección para un comando y la añade a la lista de redirecciones 
del comando. Primero, la función toma como parámetros el comando (`cmd`),
al que se le va a añadir la redirección, el tipo de redirección (`type`, 
que puede ser salida `>`, append `>>`, entrada `<` o heredoc `<<`),
y el archivo (`file`) que será usado en esa redirección.
La función empieza reservando memoria para una nueva estructura de redirección
con `malloc`, donde guardamos el tipo de redirección y duplicamos la ruta del archivo
con `ft_strdup` para asegurarnos de que tenemos una copia segura de esa cadena. 
Si ya existen redirecciones en este comando, recorremos la lista de redirecciones 
hasta encontrar el último elemento, y ahí añadimos la nueva redirección.
Si no hay redirecciones previas, simplemente la asignamos como la primera de la lista.
Al final, devuelve `0` si todo ha ido bien o `-1` si algo falla,
como si no se puede asignar memoria con `malloc` o `ft_strdup`.
*/
int	create_and_add_redirection(t_cmd *cmd, int type, char *file)
{
	t_redir	*new_redir;                                                                      // Puntero para la nueva redirección
	t_redir	*current;                                                                       // Puntero para recorrer la lista de redirecciones existentes

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

/*
Esta función se encarga de procesar una redirección en un comando y añadirla
a la lista de redirecciones del mismo. Recibe como parámetros 
el array de tokens (`tokens`), el índice actual (`i`), y el comando (`cmd`) 
al que se le va a añadir la redirección. Primero, detecta el tipo de redirección 
llamando a `get_redirection_type`, que identifica si el token actual es una 
redirección válida (como `>`, `>>`, `<`, `<<`). Si no es una redirección válida,
devuelve `1` para que el programa siga procesando otros tokens.
Si es una redirección, avanza al siguiente token, que debería ser el archivo de redirección,
y si no lo encuentra, muestra un mensaje de error.
Luego, llama a `create_and_add_redirection` para crear la redirección
y asociarla al comando. Si la creación falla, devuelve `-1`.
Finalmente, avanza el índice para continuar procesando el resto del input y
devuelve `0` si todo fue correctamente procesado.
*/
int	process_redirection(char **tokens, int *i, t_cmd *cmd)
{
	int		type;                                                                            // Variable para almacenar el tipo de redirección

	type = get_redirection_type(tokens[*i]);                                                 // Detecta el tipo de redirección del token actual
	if (type == -1)                                                                          // Si no es una redirección válida
		return (1);                                                                          // Retorna 1 para continuar con otros tokens
	if (*i > 0 && get_redirection_type(tokens[*i - 1]) != -1)                                // Verifica si hay un comando antes de la redirección
	{
		printf("Erroraso primo!: falta comando antes de la redirección.\n");                 // Mensaje de error si no hay comando antes de la redirección
		return (-1);                                                                         // Retorna -1 en caso de error
	}
	(*i)++;                                                                                  // Avanza al siguiente token, que debería ser el archivo de redirección
	if (!tokens[*i])                                                                         // Verifica si el archivo de redirección no está presente
	{
		printf("Erroraso ZUPri!: falta archivo de redirección.\n");                          // Mensaje de error si falta el archivo
		return (-1);                                                                         // Retorna -1 en caso de error
	}
	if (create_and_add_redirection(cmd, type, tokens[*i]) == -1)                             // Crea la redirección y la añade al comando
		return (-1);                                                                         // Retorna -1 si hubo error al crear la redirección
	(*i)++;                                                                                  // Avanza el índice para continuar procesando tokens
	return (0);                                                                              // Retorna 0 si todo fue correctamente procesado
}

/*
POR SI ACASO HICIERA FALTA LIBERAR O PROBLEMAS DE MEMORIA CON LAS REDIRECCIONES
*/
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