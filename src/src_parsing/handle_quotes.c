/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_quotes.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albelope <albelope@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/12 20:32:11 by albelope          #+#    #+#             */
/*   Updated: 2024/12/01 13:00:27 by albelope         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	heredoc_quoted(const char *str)
{
	if (!str)
		return (0);
	if (str[0] == '\'' && str[strlen(str) - 1] == '\'')
		return (1);
	if (str[0] == '"' && str[strlen(str) - 1] == '"')
		return (2);
	return (0);
}
