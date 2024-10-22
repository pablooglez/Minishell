/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pabloglez <pabloglez@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 17:17:32 by pabloglez         #+#    #+#             */
/*   Updated: 2024/10/22 17:17:53 by pabloglez        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_valid_identifier(const char *str)
{
	if (!str || (!ft_isalpha(str[0]) && str[0] != '_'))										//Verifica si la cadena es NULL o si el primer carácter no es una letra o guion bajo
		return (0);																			//Retorna 0 (falso) si no es un identificador válido
	int	i;																					//Inicializa un índice para comenzar a verificar desde el segundo carácter
	i = 1;
	while (str[i])																			//Recorre todos los caracteres de la cadena hasta el final
	{
		if (!ft_isalnum(str[i]) && str[i] != '_')											//Verifica si cada carácter es alfanumérico o un guion bajo
			return (0);																		//Si encuentra un carácter no válido, retorna 0
		i++;																				//Avanza al siguiente carácter
	}
	return (1);																				//Si todos los caracteres son válidos, retorna 1 (verdadero)
}