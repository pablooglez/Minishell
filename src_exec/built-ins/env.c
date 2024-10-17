/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pabloglez <pabloglez@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/14 20:00:41 by pabloglez         #+#    #+#             */
/*   Updated: 2024/10/17 19:37:17 by pabloglez        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_env(t_minishell *shell)											//Función para manejar el comando "env"
{
	t_env *current_env;														//Se inicializa "current_env" con la lista de variables de entorno del shell

	current_env = shell->env_vars;
	while (current_env)														//Bucle para recoger toda la lista de variables de entorno
	{
		if (current_env->key && current_env->value)							//Si tanto "key" como "value" son válidos (no NULL), imprime "KEY = VALUE"
			printf("%s=%s\n", current_env->key, current_env->value);
		current_env = current_env->next;									//Avanza al siguiente nodo de la lista
	}
}