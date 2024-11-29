/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_pipes.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albelope <albelope@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/15 12:43:54 by albelope          #+#    #+#             */
/*   Updated: 2024/11/29 01:15:40 by albelope         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_pipe_errors(char **tokens, int *i)
{
	if (tokens[*i] && ft_strlen(tokens[*i]) >= 2
		&& tokens[*i][0] == '|' && tokens[*i][1] == '|')
	{
		print_error("Minishell: syntax error near unexpected token `||'\n");
		return (SYNTAX_ERROR);
	}
	if (*i == 0)
	{
		print_error("Minishell: syntax error near unexpected token `|'\n");
		return (SYNTAX_ERROR);
	}
	if (!tokens[*i + 1])
	{
		print_error("Minishell: syntax error near unexpected token `|'\n");
		return (SYNTAX_ERROR);
	}
	if (tokens[*i + 1] && ft_strncmp(tokens[*i + 1], "|", 2) == 0)
	{
		print_error("Minishell: syntax error near unexpected token `|'\n");
		return (SYNTAX_ERROR);
	}
	return (0);
}

int	create_next_command(t_cmd **cmd, t_minishell *shell)
{
	(*cmd)->next = create_new_command(shell);
	if (!(*cmd)->next)
		return (-1);
	(*cmd)->next->prev = *cmd;
	*cmd = (*cmd)->next;
	if (*cmd == NULL)
		return (-1);
	return (0);
}

int	process_token_pipe(char **tokens, int *i, t_cmd **cmd, t_minishell *shell)
{
	if (tokens[*i] && ft_strncmp(tokens[*i], "|", 2) == 0)
	{
		if (check_pipe_errors(tokens, i) == SYNTAX_ERROR)
		{
			shell->exit_status = 2;
			return (-1);
		}
		if (create_next_command(cmd, shell) == -1)
			return (-1);
		(*cmd)->prev->type = PIPE;
		(*cmd)->prev->pipe[0] = -1;
		(*cmd)->prev->pipe[1] = -1;
		(*i)++;
		return (0);
	}
	return (1);
}
