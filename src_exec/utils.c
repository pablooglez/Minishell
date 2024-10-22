/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pabloglez <pabloglez@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 17:17:32 by pabloglez         #+#    #+#             */
/*   Updated: 2024/10/22 22:31:39 by pabloglez        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_env_value(t_env *env_list, char *key)									//Función para buscar una variable de entorno de la lista
{
	t_env	*current_env = env_list;												//Inicializa un puntero que apunta al primer nodo de la lista de variables de entorno.

	while(current_env)																//Inicia un bucle que continuará mientras haya nodos en la lista.
	{
		if (ft_strncmp(current_env->key, key, ft_strlen(key) + 1) == 0)				//Compara la clave actual con la clave buscada.
			return (current_env->value);											//Si hay una coincidencia, retorna el valor asociado a la clave encontrada.
		current_env = current_env->next;											//Avanza al siguiente nodo en la lista de variables de entorno.
	}
	return (NULL);																	//Si no se encuentra la clave, retorna NULL.
}

void	update_env_var(t_env **env_list, char *key, char *value)					//Función para buscar o añadir una variable de entorno en la lista
{
	t_env	*current_env = *env_list;												//Inicializa un puntero que apunta al primer nodo de la lista de variables de entorno.
	
	while (current_env)																//Inicia un bucle que continuará mientras haya nodos en la lista.
	{
		if (ft_strncmp(current_env->key, key, ft_strlen(key) + 1) == 0)				//Compara la clave actual con la clave buscada.
		{
			free(current_env->value);												//Libera la memoria del valor anterior de la variable de entorno.
			current_env->value = ft_strdup(value);									//Asigna el nuevo valor a la variable de entorno.
			return;																	//Sale de la función después de actualizar la variable.
		}
		current_env = current_env->next;											//Avanza al siguiente nodo en la lista de variables de entorno.
	}
																					//Si no se encuentra la variable, crear una nueva y añadirla a la lista.
	t_env *new_env_var = (t_env *)malloc(sizeof(t_env));							//Reserva memoria para un nuevo nodo.
	new_env_var->key = ft_strdup(key);												//Copia la clave de la variable de entorno.
	new_env_var->value = ft_strdup(value);											//Copia el valor de la variable de entorno.
	new_env_var->next = *env_list;													//El nuevo nodo apunta al nodo actual que es la cabeza de la lista.
	new_env_var->prev = NULL;														//No hay nodo anterior, ya que este será el nuevo nodo de cabeza.
	if (*env_list)																	//Si la lista no estaba vacía...
		(*env_list)->prev = new_env_var;											//Actualiza el puntero 'prev' del nodo anterior para apuntar al nuevo nodo.
	*env_list = new_env_var;														//Actualiza la cabeza de la lista para que apunte al nuevo nodo.
}

int	is_valid_identifier(const char *str)
{
	if (!str || (!ft_isalpha(str[0]) && str[0] != '_'))								//Verifica si la cadena es NULL o si el primer carácter no es una letra o guion bajo
		return (0);																	//Retorna 0 (falso) si no es un identificador válido
	int	i;																			//Inicializa un índice para comenzar a verificar desde el segundo carácter
	i = 1;
	while (str[i])																	//Recorre todos los caracteres de la cadena hasta el final
	{
		if (!ft_isalnum(str[i]) && str[i] != '_')									//Verifica si cada carácter es alfanumérico o un guion bajo
			return (0);																//Si encuentra un carácter no válido, retorna 0
		i++;																		//Avanza al siguiente carácter
	}
	return (1);																		//Si todos los caracteres son válidos, retorna 1 (verdadero)
}