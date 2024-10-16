/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_redirections.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albelope <albelope@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/16 11:57:47 by albelope          #+#    #+#             */
/*   Updated: 2024/10/16 16:37:27 by albelope         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"


/* La función se encarga de identificar qué tipo de redirección es, es decir, si
   es una redirección de salida (>), de salida en modo append (>>), de entrada (<),
   o un here-document (<<). Si no es ninguna de estas, devuelve -1, indicando
   que no es una redirección válida. */
int	get_redirection_type(char *token)
{
	if (!token)
		return (-1);
	if (ft_strncmp(token, ">", 1) == 0 && ft_strlen(token) == 1)
		return (REDIR_OUT);
	else if (ft_strncmp(token, ">>", 2) == 0 && ft_strlen(token) == 2)
		return (REDIR_APPEND);
	else if (ft_strncmp(token, "<", 1) == 0 && ft_strlen(token) == 1)
		return (REDIR_IN);
	else if (ft_strncmp(token, "<<", 2) == 0 && ft_strlen(token) == 2)
		return (REDIR_HEREDOC);
	return (-1);
}
/*
Crea una nueva redirección para un comando y la añade a la lista de redirecciones 
del comando. Primero, la función toma como parámetros el comando (`cmd`),
al que se le va a añadir la redirección, el tipo de redirección (`type`, 
que puede ser salida `>`, append `>>`, entrada `<` o heredoc `<<`),
y el archivo (`file`) que será usado en esa redirección.
La función empieza reservando memoria para una nueva estructura de redirección
con `malloc`, donde guardamos el tipo de redirección y duplicamos la ruta del archivo
con `ft_strdup` para asegurarnos de que tenemos una copia segura de esa cadena. 
Si ya existen redirecciones en este comando, recorremos la lista de redirecciones 
hasta encontrar el último elemento, y ahí añadimos la nueva redirección.
Si no hay redirecciones previas, simplemente la asignamos como la primera de la lista
Al final, devuelve `0` si todo ha ido bien o `-1` si algo falla,
como si no se puede asignar memoria con `malloc` o `ft_strdup`.
*/

int	create_and_add_redirection(t_cmd *cmd, int type, char *file)
{
	t_redir	*new_redir;
	t_redir	*current;

	new_redir = malloc(sizeof(t_redir));
	if (!new_redir)
		return (-1);
	new_redir->type = type;
	new_redir->ruta = ft_strdup(file);
	if (!new_redir->ruta)
	{
		free(new_redir);
		return (-1);
	}
	new_redir->next = NULL;
	if (!cmd->redir)
		cmd->redir = new_redir;
	else
	{
		current = cmd->redir;
		while (current->next)
			current = current->next;
		current->next = new_redir;
	}
	return (0);
}

/*
Esta función se encarga de procesar una redirección en un comando y añadirla
a la lista de redirecciones del mismo. Recibe como parámetros 
el array de tokens (`tokens`), el índice actual (`i`), y el comando (`cmd`) 
al que se le va a añadir la redirección. Primero, detecta el tipo de redirección 
llamando a `get_redirection_type`, que identifica si el token actual es una 
redirección válida (como `>`, `>>`, `<`, `<<`). Si no es una redirección válida,
devuelve `1` para que el programa siga procesando otros tokens.
Si es una redirección, avanza al siguiente token, que debería ser el archivo de redirección,
y si no lo encuentra, muestra un mensaje de error.
Luego, llama a `create_and_add_redirection` para crear la redirección
y asociarla al comando. Si la creación falla, devuelve `-1`.
Finalmente, avanza el índice para continuar procesando el resto del input y
devuelve `0` si todo fue correctamente procesado.
*/

int	process_redirection(char **tokens, int *i, t_cmd *cmd)
{
	int		type;

	type = get_redirection_type(tokens[*i]);
	if (type == -1)
		return (1);  // No es una redirección válida
	(*i)++;
	if (!tokens[*i])
	{
		printf("Error: falta archivo de redirección.\n");
		return (-1);
	}
	if (create_and_add_redirection(cmd, type, tokens[*i]) == -1)
		return (-1);
	(*i)++;
	return (0);
}

/*
POR SI ACASO HICIERA FALTA LIBERAR O PROBLEMAS DE MEMORIA CON LAS REDIRECCIONES
*/
void	free_redirections(t_redir *redir)
{
	t_redir	*temp;

	while (redir)
	{
		temp = redir;
		redir = redir->next;
		free(temp->ruta);
		free(temp);
	}
}


