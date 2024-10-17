/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pabloglez <pabloglez@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/08 16:48:22 by pabloglez         #+#    #+#             */
/*   Updated: 2024/10/15 20:48:12 by pabloglez        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void free_array(char ***array)
{
	int i;
	i = -1;									// Inicializa el índice 'i' en -1.

	if (*array)								// Comprueba si el puntero a la matriz (array) no es NULL.
	{
		while ((*array)[++i])				// Recorre cada puntero de la matriz de cadenas hasta encontrar NULL.
			free((*array)[i]);				// Libera la memoria asignada a cada cadena de la matriz.
		free(*array);						// Libera el puntero principal de la matriz.
		*array = NULL;						// Asigna NULL al puntero principal para evitar punteros colgantes.
	}
}

void free_redir(t_redir **redir)
{
	t_redir *next;

	while (*redir)							// Recorre la lista enlazada de estructuras `t_redir`.
	{
		next = (*redir)->next;				// Almacena el siguiente nodo.
		if (next)							// Si hay un siguiente nodo:
			next->prev = NULL;				// Elimina el enlace hacia el nodo actual desde el siguiente nodo.
		if ((*redir)->file)					// Si el campo `file` no es NULL:
		{
			free((*redir)->file);			// Libera la memoria de `file`.
			(*redir)->file = NULL;			// Asigna NULL a `file` para evitar punteros colgantes.
		}
		if ((*redir)->fd > 0)				// Si el descriptor de archivo es válido:
			close((*redir)->fd);			// Cierra el archivo asociado.
		(*redir)->fd = 0;					// Restablece el valor de `fd`.
		free(*redir);						// Libera el nodo actual.
		*redir = NULL;						// Evita un puntero colgante.
		*redir = next;						// Avanza al siguiente nodo en la lista.
	}
}


void free_env_list(t_env **env_var)
{
	t_env *next;

	while((*env_var))						// Recorre la lista enlazada de variables de entorno.
	{
		next = (*env_var)->next;			// Almacena el siguiente nodo.
		if (next)							// Si hay un siguiente nodo:
			next->prev = NULL;				// Elimina el enlace hacia el nodo actual desde el siguiente nodo.
		free((*env_var)->key);				// Libera la memoria del campo `key`.
		free((*env_var)->value);			// Libera la memoria del campo `value`.
		free((*env_var));					// Libera la estructura `env_var`.
		(*env_var) = NULL;					// Evita punteros colgantes.
		(*env_var) = next;					// Avanza al siguiente nodo en la lista.
	}
}


void *free_shell(t_minishell **shell)
{
	if ((*shell)->env)						// Si la matriz de entorno (`env`) existe:
		free_array(&(*shell)->env);			// Libera la memoria de la matriz de entorno.

	if ((*shell)->env_vars)					// Si la lista de variables de entorno existe:
		free_env_list(&(*shell)->env_vars); // Libera la memoria de la lista de variables de entorno.

	close((*shell)->original_stdin);		// Cierra el descriptor del `stdin` original.
	close((*shell)->original_stdout);		// Cierra el descriptor del `stdout` original.
	free(*shell);							// Libera la estructura `shell`.
	return (NULL);							// Retorna NULL como buena práctica al liberar memoria.
}


void *free_tokens(t_cmd **tokens)
{
	t_cmd *next;

	while (*tokens)							// Recorre la lista de comandos (tokens).
	{
		next = (*tokens)->next;				// Almacena el siguiente nodo.
		if (next)							// Si hay un siguiente nodo:
			next->prev = NULL;				// Elimina el enlace hacia el nodo actual desde el siguiente nodo.
		free_array(&(*tokens)->arguments);	// Libera la matriz de argumentos de este token.
		free_redir(&(*tokens)->redir);		// Libera las redirecciones asociadas a este token.
		free(*tokens);						// Libera el token actual.
		*tokens = NULL;						// Evita punteros colgantes.
		*tokens = next;						// Avanza al siguiente token.
	}
	return (NULL);							// Retorna NULL tras liberar todos los tokens.
}
