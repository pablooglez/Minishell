/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albelope <albelope@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/11 20:39:51 by albelope          #+#    #+#             */
/*   Updated: 2024/10/30 20:10:13 by albelope         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	free_tokens_parse(char **tokens)
{
	int	i;

	i = 0;
	if (!tokens)
		return ;
	while (tokens[i])
		free(tokens[i++]);
	free(tokens);
}

int	handle_token(char *input, int i, char **tokens, int *j)
{
	int	start;

	start = i;
	while (input[i] && input[i] != ' ' && !is_quote(input[i]) && get_special_token_type(input[i]) == UNKNOWN)
		i++;
	tokens[*j] = ft_substr(input, start, i - start);
	if (!tokens[*j])
	{
		free_tokens_parse(tokens);
		return (-1);
	}
	(*j)++;
	return (i);
}

int	process_token(char *input, int *i, char **tokens, int *j)
{
	if (is_quote(input[*i]))
	{
		*i = handle_quotes(input, *i, tokens, j);
		if (*i == -1)
			return (-1);
	}
	else if (get_special_token_type(input[*i]) != UNKNOWN)
	{
		*i = handle_special_char(input, *i, tokens, j);
		if (*i == -1)
			return (-1);
	}
	else
	{
		*i = handle_token(input, *i, tokens, j);
		if (*i == -1)
			return (-1);
	}
	return (0);
}

char	**tokenize_input(char *input)
{
	char	**tokens;
	int		i;
	int		j;

	i = 0;
	j = 0;
	tokens = ft_calloc(100, sizeof(char *));
	if (!tokens)
		return (NULL);
	while (input[i])
	{
		while (input[i] == ' ')
			i++;
		if (process_token(input, &i, tokens, &j) == -1)
		{
			free_tokens_parse(tokens);
			return (NULL);
		}
		//while (input[i] == ' ')
			//i++;
	}
	tokens[j] = NULL;
	return (tokens);
}
