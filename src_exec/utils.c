/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pabloglez <pabloglez@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 17:17:32 by pabloglez         #+#    #+#             */
/*   Updated: 2024/10/23 20:17:25 by pabloglez        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char *get_command_path(char *cmd, t_minishell *shell)
{
	char	*path_env;																// Variable para almacenar el valor de la variable de entorno PATH.
	char	**paths;																// Array de cadenas que contendrá las diferentes rutas en PATH.
	char	*full_path;																// Ruta completa del comando después de añadir el nombre del comando.
	char	*temp_path;																// Variable temporal para formar las rutas durante la iteración.
	int		i;																		// Contador para iterar sobre las rutas.

	path_env = get_env_value(shell->env_vars, "PATH");								// Busca la variable de entorno "PATH" dentro de las variables de entorno del shell.
	if (!path_env)																	// Si no se encuentra la variable PATH, devuelve NULL, ya que no hay rutas disponibles para buscar el comando.
		return (NULL);

	paths = ft_split(path_env, ':');												// Divide la cadena PATH en un array de rutas usando ':' como delimitador.
	if (!paths)																		// Si la división falla o no hay rutas, devuelve NULL.
		return (NULL);

	i = 0;																			// Inicializa el índice para recorrer las rutas en el array paths.

	while (paths[i])																// Recorre cada ruta en el array de paths.
	{
		temp_path = ft_strjoin(paths[i], "/");										// Forma una ruta temporal concatenando la ruta actual con un '/'.
		if (!temp_path)																// Si falla la creación de la ruta temporal, libera la memoria de paths y retorna NULL.
		{
			free_array(&paths);
			return (NULL);
		}
		full_path = ft_strjoin(temp_path, cmd);										// Completa la ruta agregando el comando al final de la ruta temporal.
		free(temp_path);															// Libera la memoria de la ruta temporal, ya que ahora se usa full_path.

		if (access(full_path, X_OK) == 0)											// Verifica si la ruta completa es accesible y ejecutable.
		{
			free_array(&paths);														// Si se encuentra el ejecutable, libera la memoria de paths.
			return (full_path);														// Devuelve la ruta completa del ejecutable.
		}
		free(full_path);															// Si la ruta no es válida, libera la memoria de full_path.
		i++;																		// Incrementa el índice para probar la siguiente ruta en paths.
	}
	free_array(&paths);																// Si no se encuentra el ejecutable en ninguna de las rutas, libera paths.
	return (NULL);																	// Devuelve NULL si no se encuentra el comando en las rutas de PATH.
}

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