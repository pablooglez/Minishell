/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_input.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albelope <albelope@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/11 19:22:39 by albelope          #+#    #+#             */
/*   Updated: 2024/10/25 13:14:04 by albelope         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

/* 
   Función que se encarga de leer la entrada del usuario en el prompt de Minishell.

   La función hace lo siguiente:
   1. Llama a la función readline() que muestra el prompt "Minishell>" al usuario
      y captura la línea de texto que el usuario introduce.
   2. Si la entrada es NULL (por ejemplo, si se recibe un EOF o el usuario presiona Ctrl+D),
      se imprime "Exit..." y se devuelve NULL para indicar que no hay más entrada.
   3. Si la entrada no está vacía, se agrega al historial de comandos usando add_history().
   4. Finalmente, devuelve la cadena de entrada capturada para ser procesada por el shell.
*/

char	*read_input(void)
{
	char	*input;                                                                             // Variable para almacenar la entrada del usuario

	input = readline(CYAN "Minishell>" WHITE);                                                   // Muestra el prompt y captura la entrada del usuario
	if (!input)                                                                                  // Verifica si la entrada es NULL (EOF o Ctrl+D)
	{
		printf("Exit....\n");                                                                    // Muestra el mensaje "Exit...."
		return (NULL);                                                                           // Retorna NULL para indicar que no hay más entrada
	}
	if (*input != '\0')                                                                          // Verifica si la entrada no está vacía
		add_history(input);                                                                      // Agrega la entrada al historial de comandos
	return (input);                                                                              // Devuelve la cadena de entrada capturada
}



// PRUEBAS FUTURAS PARA MANEJAR SI EL ULTIMA CARACTER DE LINEA ES UN BACKSLASH //
// SI ESTA AL FINAL DE UNA LINEA, DEBERIA SER TRATADO COMO UN CARACTER DE ESCAPE...//
// QUE INDICA QUE EL COMANDO SE CONTINUARÁ EN LA SIGUIENTE LINEA //