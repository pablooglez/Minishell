/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pabloglez <pabloglez@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/15 18:05:45 by pabloglez         #+#    #+#             */
/*   Updated: 2024/10/15 20:48:05 by pabloglez        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void *safe_malloc(t_minishell *shell, size_t size)
{
	void *tmp;														// Declaración de un puntero para almacenar la dirección de memoria asignada

	tmp = malloc(size);											    // Intento de asignar 'size' bytes de memoria
	if (!tmp)														// Si la asignación de memoria falla (tmp es NULL)
		ft_error(shell, MEMORY, NULL, 1);							// Llama a la función ft_error para manejar el error, pasando el shell y un código de error
	return (tmp);													// Devuelve el puntero a la memoria asignada (o NULL si falló)
}

void create_env_vars(t_minishell *shell, char **env)
{
    int i;
    i = 0;                                                          // Inicialización del índice a 0.

     while (env && env[i] != NULL)                                  // Mientras 'env' no sea NULL y la cadena de entorno en la posición 'i' no sea NULL.
    {
        t_env *node;                                                // Declaración de un puntero a un nodo de tipo t_env.
        node = safe_malloc(shell, sizeof(t_env));                   // Llama a safe_malloc para asignar memoria para un nuevo nodo t_env.

                                                                    // Dividir la cadena en clave y valor usando '='
        char *equal_sign = ft_strchr(env[i], '=');                  // Buscar el carácter '='.
        if (equal_sign)                                             // Si se encuentra el carácter '='.
        {
            node->key = ft_strndup(env[i], equal_sign - env[i]);    // Copiar la clave (parte antes del '=').
            node->value = ft_strdup(equal_sign + 1);                // Copiar el valor (parte después del '=').
        }
        else                                                        // Si no hay '='.
        {
            node->key = ft_strdup(env[i]);                          // Copiar la cadena completa como clave.
            node->value = NULL;                                     // Sin valor asociado.
        }

                                                                    // Añadir el nuevo nodo a la lista de variables de entorno.
        node->next = shell->env_vars;                               // El nuevo nodo apunta a la cabeza de la lista actual.
        node->prev = NULL;                                          // No hay nodo anterior porque este es el nuevo primer nodo.
        if (shell->env_vars)                                        // Si ya hay nodos en la lista.
            shell->env_vars->prev = node;                           // Ajustar el puntero 'prev' del nodo existente.

        shell->env_vars = node;                                     // Actualizar la cabeza de la lista para que apunte al nuevo nodo.
        i++;                                                        // Avanzar al siguiente elemento de 'env'.
    }
}
