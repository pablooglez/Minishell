/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   memory.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pabloglez <pabloglez@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/31 15:08:15 by albelope          #+#    #+#             */
/*   Updated: 2024/11/04 20:54:04 by pabloglez        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_tokens_parse(char **tokens)	// Define la función free_tokens_parse que toma un array de strings (tokens) como parámetro
{
	int	i;									// Declara una variable entera i para usarla como índice

	i = 0;									// Inicializa i en 0
	if (!tokens)							// Verifica si tokens es NULL (es decir, si no hay nada que liberar)
		return ;							// Si tokens es NULL, sale de la función sin hacer nada
	while (tokens[i])						// Mientras haya un token en la posición i
		free(tokens[i++]);					// Libera el token actual y luego incrementa i para liberar el siguiente
	free(tokens);							// Finalmente, libera el array de punteros en sí mismo
}
