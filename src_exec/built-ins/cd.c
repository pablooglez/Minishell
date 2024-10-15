/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pabloglez <pabloglez@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/14 19:54:51 by pabloglez         #+#    #+#             */
/*   Updated: 2024/10/15 18:48:57 by pabloglez        ###   ########.fr       */
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

void	ft_cd(t_minishell *shell, char **arg)										//Función para manejar el comando "cd" (cambiar directorio)
{
	char	*dir;																	//Variable para almacenar el directorio al que se desea cambiar.
	char	cwd[PATH_MAX];															//Buffer para almacenar el directorio actual.

	if (!arg[1])																	//Si no se proporciona un argumento (es decir, el usuario solo escribió 'cd')...
	{
		dir = get_env_value(shell->env_vars, "HOME");														//Obtener el valor de la variable de entorno HOME.
		if (!dir)																	//Si no encuentra la variable HOME...
		{
			ft_error(shell, CMD_NOT_FOUND, "cd: HOME not set", 0);					//Mostrar un error indicando que HOME no está configurado.
			return;																	//Salir de la función.
		}
	}
	else
		dir = arg[1];																//Si hay un argumento, usarlo como el directorio al que se desea cambiar.
	
	if (chdir(dir) != 0)															//Intentar cambiar al directorio especificado.
	{
		ft_error(shell, CMD_NOT_FOUND, dir, 0);										//Si falla, mostrar un error indicando que no se puede cambiar al directorio.
		return;																		//Salir de la función
	}
	if (getcwd(cwd, sizeof(cwd)) != NULL)											//Obtener el directorio actual y almacenarlo en cwd.
		update_env_var(&(shell->env_vars), "PWD", cwd);								//Actualizar la variable de entorno PWD en la lista de variables.
	else
		ft_error(shell, CMD_NOT_FOUND, "cd: failed to get current directory", 0);	//Mostrar un error si falla getcwd().
}
