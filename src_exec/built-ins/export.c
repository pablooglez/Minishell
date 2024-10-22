/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pabloglez <pabloglez@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/14 20:00:11 by pabloglez         #+#    #+#             */
/*   Updated: 2024/10/22 17:18:02 by pabloglez        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_env_vars(t_env *env_list)														//Función auxiliar para imprimir las variables de entorno
{
	t_env	*current_env = env_list;														//Inicializa un puntero que apunta a la lista de variables de entorno

	while (current_env)																		//Bucle para recorrer toda la lista de variables de entorno
	{
		if (current_env->key)																//Verifica que la clave de la variable de entorno existe
		{
			if (current_env->value)															//Si la variable tiene un valor asociado
				printf("declare -x %s=\"%s\"\n", current_env->key, current_env->value);		//Imprimir en formato: declare -x KEY="VALUE"
			else
				printf ("declare -x %s\n", current_env->key);								//Si no tiene valor, imprime solo la clave: declare -x KEY
		}
		current_env = current_env->next;													//Avanza al siguiente nodo de la lista de variables de entorno
	}
}

void	ft_export(t_minishell *shell, char **arg)
{
	int	i;

	i = 1;																					//Inicializa el índice a 1 pq arg[0] es el propio comando "export"
	
	if (!arg[1])																			//Si no hay argumentos después de "export", imprime todas las variables de entorno
	{
		print_env_vars(shell->env_vars);													//Llama a la función auxiliar para imprimir todas las variables de entorno
		return;																				//Sale de la función después de imprimir
	}

	while (arg[i])																			//Bucle para recorrer los argumentos que siguen al comando "export"
	{
		char *equal_sign = ft_strchr(arg[i], '=');											//Busca el simbolo '=' en el argumento actual

		if (equal_sign)																		//Si encuentra un '=' en el argumento, significa que es el tipo "KEY=VALUE"
		{
			char *key = ft_strndup(arg[i], equal_sign - arg[i]);							//Extrae la clave (lo que está antes del '=')
			char *value = ft_strdup(equal_sign + 1);										//Extrae el valor (lo que está después del '=')
			update_env_var(&(shell->env_vars), key, value);									//Actualiza o añade la variable de entorno con clave y valor
			free(key);																		//Libera la memoria asignada a la clave
			free(value);																	//Libera la memoria asignada al valor
		}
		else																				//Si no hay un '=', es solo un nombre de variable sin valor
		{
			if (is_valid_identifier(arg[i]))												//Verifica si es un identificador válido (ejemplo: solo letras y guiones bajos)
				update_env_var(&(shell->env_vars), arg[i], NULL);							//Añade la variable sin valor asociado (se establece a NULL)
			else
				ft_error(shell, CMD_NOT_FOUND, "export: not a valid identifier", 0);		//Muestra un error si el identificador no es válido
		}
		i++;																				//Avanza al siguiente argumento en la lista
	}
}