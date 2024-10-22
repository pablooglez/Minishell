/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pabloglez <pabloglez@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/14 20:00:48 by pabloglez         #+#    #+#             */
/*   Updated: 2024/10/22 18:00:59 by pabloglez        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	delete_env_var(t_env **env_list, const char *key)						//Función auxiliar para eliminar una variable de entorno
{
	t_env *current_env = *env_list;												//Inicializa un puntero que apunta al primer nodo de la lista de variables de entorno

	while (current_env)															//Recorre la lista de variables de entorno
	{
																				//Usa ft_strncmp para comparar la clave de la variable actual con 'key'
		if (ft_strncmp(current_env->key, key, ft_strlen(key) + 1) == 0)			//Compara la clave con 'key'
		{
																				//Ajusta los punteros para eliminar el primer nodo de la lista
			if (current_env->prev)												//Si hay un nodo anterior
				current_env->prev->next = current_env->next;					//Conecta el nodo anterior con el siguiente
			
			if (current_env->next)												//Si no hay un nodo siguiente
				current_env->next->prev = current_env->prev;					//Conecta el nodo siguiente con el anterior
			
			if (current_env == *env_list)										//Si el nodo a eliminar es el primero de la lista
				*env_list = current_env->next;									//Actualiza la cabeza de la lista
			
																				//Libera la memoria del nodo eliminado
			free(current_env->key);
			free(current_env->value);
			free(current_env);
			return;																//Sale de la función una vez eliminada la variable
		}
		current_env = current_env->next;										//Avanza al siguiente nodo de la lista
	}
}

void	ft_unset(t_minishell *shell, char **arg)								//Función auxiliar para eliminar el comando unset
{
	int	i;
	
	i = 1;																		//Empiezza en el seundo argumento, ya que arg[0] es "unset"

	if (!arg[1])																//Si no se proporciona ningún argumento, no hace nada
		return;

	while (arg[i])																//Recorre todos los argumentos proporcionados
	{
		if (is_valid_identifier(arg[i]))										//Verifica si el argumento es un identificador válido
			delete_env_var(&(shell->env_vars), arg[i]);							//Llama a la función para eliminar la variable de entorno
		
		else																	//Si el nombre de la variable no es válido
			ft_error(shell, CMD_NOT_FOUND, "unset: not a valid identifier", 0); //Muestra un error
		i++;																	//Avanza al siguiente argumento
	}
}