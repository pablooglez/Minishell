/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albelope <albelope@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/13 17:56:57 by albelope          #+#    #+#             */
/*   Updated: 2024/10/16 18:19:04 by albelope         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*#include "../include/minishell.h"

void print_tokens(char **tokens)
{
    int i = 0;
    while (tokens[i])
    {
        printf("Token[%d]: %s\n", i, tokens[i]);
        i++;
    }
}

int main(void)
{
    char *input;
    char **tokens;

    while (1)
    {
        input = read_input();  // Leer la entrada del usuario
        if (!input)
            break;

        tokens = tokenize_input(input);  // Tokenizar la entrada
        if (tokens)  // Si la tokenización fue exitosa
        {
            print_tokens(tokens);  // Imprimir los tokens
            free_tokens(tokens);  // Liberar tokens
        }
        free(input);
    }

    return 0;
}*/