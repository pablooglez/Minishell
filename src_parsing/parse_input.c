/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_input.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pabloglez <pabloglez@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/11 21:23:53 by albelope          #+#    #+#             */
/*   Updated: 2024/11/04 21:05:49 by pabloglez        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_cmd	*initialize_first_command(t_minishell *shell)				// Define la función que inicializa el primer comando
{
	t_cmd	*cmd;													// Declara un puntero a t_cmd que representará el comando

	cmd = create_new_command(shell);								// Crea un nuevo comando usando la función create_new_command y lo asigna a cmd
	if (!cmd)														// Si cmd es NULL (no se pudo crear el comando)
		return (NULL);												// Retorna NULL para indicar un error
	return (cmd);													// Retorna el puntero al nuevo comando creado
}

int	initialize_arguments(char **tokens, int *i, t_cmd *cmd)			// Inicializa los argumentos de un comando en cmd
{
	if (!cmd->path)													// Si cmd->path es NULL (no hay un path establecido aún)
	{
		cmd->path = ft_strdup(tokens[*i]);							// Copia el token actual a cmd->path
		if (!cmd->path)												// Si la copia falla
			return (-1);											// Retorna -1 para indicar un error
		(*i)++;														// Incrementa el índice i para pasar al siguiente token
	}
	cmd->arguments = ft_calloc(100, sizeof(char *));				// Reserva memoria para hasta 100 argumentos
	if (!cmd->arguments)											// Si la asignación falla
		return (-1);												// Retorna -1 para indicar un error
	return (0);														// Retorna 0 para indicar éxito
}

int	add_argument(char *token, int arg_index, t_cmd *cmd)			// Agrega un argumento a cmd->arguments en la posición arg_index
{
	cmd->arguments[arg_index] = ft_strdup(token);					// Copia el token y lo asigna en cmd->arguments[arg_index]
	if (!cmd->arguments[arg_index])									// Si la copia falla
		return (-1);												// Retorna -1 para indicar un error
	return (0);														// Retorna 0 para indicar éxito
}


/*int	process_arguments(char **tokens, int *i, t_cmd *cmd, t_minishell *shell)
{
	int	arg_index;
	int	ret;

	if (initialize_arguments(tokens, i, cmd) == -1)
		return (-1);
	arg_index = 0;
	while (tokens[*i] && ft_strncmp(tokens[*i], "|", 2) != 0)
	{
		if (get_redirection_type(tokens[*i]) != NOT_REDIR)
		{
			ret = process_redirection(tokens, i, cmd, shell);
			if (ret == -1)
				return (-1);
		}
		else
		{
			if (add_argument(tokens[*i], arg_index, cmd) == -1)
				return (-1);
			arg_index++;
			(*i)++;
		}
	}
	cmd->arguments[arg_index] = NULL;
	return (0);
}*/
int	process_arguments(char **tokens, int *i, t_cmd *cmd, t_minishell *shell)	// Procesa los argumentos de un comando
{
	int	arg_index;																// Declara el índice de argumentos
	int	ret;																	// Declara una variable para almacenar resultados de funciones

	if (initialize_arguments(tokens, i, cmd) == -1)								// Inicializa argumentos y verifica si falla
		return (-1);															// Retorna -1 en caso de error
	arg_index = 0;																// Inicializa el índice de argumentos en 0
	cmd->arguments[arg_index] = ft_strdup(cmd->path);							// Copia cmd->path al primer argumento
	if (!cmd->arguments[arg_index])												// Si la copia falla
		return (-1);															// Retorna -1 para indicar un error

	arg_index++;																// Incrementa el índice de argumentos
	while (tokens[*i] && ft_strncmp(tokens[*i], "|", 2) != 0)					// Mientras haya tokens y no sea un pipe ("|")
	{
		if (get_redirection_type(tokens[*i]) != NOT_REDIR)						// Si el token es una redirección
		{
			ret = process_redirection(tokens, i, cmd, shell);					// Procesa la redirección
			if (ret == -1)														// Si la redirección falla
				return (-1);													// Retorna -1 para indicar un error
		}
		else
		{
			if (add_argument(tokens[*i], arg_index, cmd) == -1)					// Agrega el token como argumento
				return (-1);													// Retorna -1 en caso de error
			arg_index++;														// Incrementa el índice de argumentos
			(*i)++;																// Incrementa el índice de tokens
		}
	}
	cmd->arguments[arg_index] = NULL;											// Agrega un NULL al final de los argumentos
	return (0);																	// Retorna 0 para indicar éxito
}

int	process_tokens(char **tokens, t_cmd *current_cmd, t_minishell *shell)		// Procesa todos los tokens de entrada
{
	int	i;																		// Declara un índice para recorrer los tokens

	i = 0;																		// Inicializa i en 0
	while (tokens[i])															// Mientras haya tokens
	{
		if (process_token_pipe(tokens, &i, &current_cmd, shell) == -1)			// Procesa posibles pipes y verifica errores
			return (-1);														// Retorna -1 en caso de error
		if (process_arguments(tokens, &i, current_cmd, shell) == -1)			// Procesa los argumentos del comando
			return (-1);														// Retorna -1 en caso de error
	}
	return (0);																	// Retorna 0 para indicar éxito
}

t_cmd	*parse_input(char *input_line, t_minishell *shell)						// Parsea la línea de entrada del usuario
{
	char	**tokens;															// Declara un array de strings para los tokens
	t_cmd	*cmd;																// Declara un puntero a t_cmd para almacenar el comando

	if (is_empty_or_whitespace(input_line))										// Verifica si la línea de entrada está vacía o es solo espacio
        return (NULL);															// Retorna NULL si no hay nada que parsear
	if (contains_invalid_characters(input_line))								// Verifica si la línea tiene caracteres inválidos
		return (NULL);															// Retorna NULL si encuentra caracteres inválidos
	tokens = tokenize_input(input_line);										// Divide la línea de entrada en tokens
	if (!tokens || !tokens[0])													// Si no se pudieron obtener tokens o está vacío
	{
		free_tokens_parse(tokens);												// Libera tokens y retorna NULL
		return (NULL);
	}
	cmd = initialize_first_command(shell);										// Inicializa el primer comando
	if (!cmd)																	// Si falla la inicialización
	{
		free_tokens_parse(tokens);												// Libera tokens y retorna NULL
		return (NULL);
	}
	if (tokens[0][0] == '$')													// Si el primer token es una variable
		cmd->path = ft_strdup("echo");											// Asigna "echo" como el path del comando
	if (process_tokens(tokens, cmd, shell) == -1)								// Procesa los tokens y verifica si hay errores
	{
		free_tokens_parse(tokens);												// Libera tokens y comando, luego retorna NULL
		free_command(cmd);
		return (NULL);
	}
	expand_tokens(cmd, shell);													// Expande las variables en el comando
	free_tokens_parse(tokens);													// Libera los tokens ya que no se necesitan más
	//print_command(cmd);														// Imprime el comando para depuración (si está habilitado)
	return (cmd);																// Retorna el comando parseado
}
