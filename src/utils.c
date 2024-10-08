/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pabloglez <pabloglez@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/08 20:02:14 by pabloglez         #+#    #+#             */
/*   Updated: 2024/10/08 20:54:25 by pabloglez        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void *safe_malloc(t_minishell *shell, size_t size)
{
	void *tmp;														// Declaración de un puntero para almacenar la dirección de memoria asignada

	tmp = malloc(size);												// Intento de asignar 'size' bytes de memoria
	if (!tmp)														// Si la asignación de memoria falla (tmp es NULL)
		ft_error(shell, MEMORY, NULL, 1);							// Llama a la función ft_error para manejar el error, pasando el shell y un código de error
	return (tmp);													// Devuelve el puntero a la memoria asignada (o NULL si falló)
}

void create_env_vars(t_minishell *shell, char **env)
{
	int i;															// Declaración de una variable entera para usar como índice
	i = 0;															// Inicialización del índice a 0
	
	while (env && env[i] != NULL) 									// Mientras 'env' no sea NULL y la cadena de entorno en la posición 'i' no sea NULL
	{
		t_env *node;												// Declaración de un puntero a un nodo de tipo t_env

		node = safe_malloc(shell, sizeof(t_env));					// Llama a safe_malloc para asignar memoria para un nuevo nodo t_env
		i++;														// Incrementa el índice para pasar al siguiente elemento de 'env'
	}
}
