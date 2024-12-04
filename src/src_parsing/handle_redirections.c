/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_redirections.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albelope <albelope@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/16 11:57:47 by albelope          #+#    #+#             */
/*   Updated: 2024/11/30 12:17:04 by albelope         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	get_redirection_type(char *token)
{
	if (!token)
		return (NOT_REDIR);
	if (ft_strncmp(token, ">", 1) == 0 && ft_strlen(token) == 1)
		return (OUTFILE);
	else if (ft_strncmp(token, ">>", 2) == 0 && ft_strlen(token) == 2)
		return (APPEND);
	else if (ft_strncmp(token, "<", 1) == 0 && ft_strlen(token) == 1)
		return (INFILE);
	else if (ft_strncmp(token, "<<", 2) == 0 && ft_strlen(token) == 2)
		return (HEREDOC);
	return (NOT_REDIR);
}

int	create_and_add_redirection(t_cmd *cmd, int type, char *file)
{
	t_redir	*new_redir;
	t_redir	*current;

	new_redir = malloc(sizeof(t_redir));
	if (!new_redir)
		return (-1);
	new_redir->type = type;
	new_redir->file = ft_strdup(file);
	new_redir->next = NULL;
	if (!new_redir->file)
	{
		free(new_redir);
		return (-1);
	}
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

int	validate_redirection_syntax(char **tokens, int *i)
{
	int	next_type;

	if (tokens[*i + 1] != NULL)
		next_type = get_redirection_type(tokens[*i + 1]);
	else
		next_type = NOT_REDIR;
	if (next_type != NOT_REDIR)
		return (-1);
	if (tokens[*i + 1] == NULL || is_empty_or_whitespace(tokens[*i + 1]))
		return (-1);
	return (0);
}

int	validate_and_expand(char **expanded_filename, char *token,
		t_minishell *shell)
{
	*expanded_filename = expand_string(token, shell);
	if (!*expanded_filename || is_empty_or_whitespace(*expanded_filename))
		return (-1);
	return (0);
}

int	process_redirection(char **tokens, int *i, t_cmd *cmd, t_minishell *shell)
{
	int		type;
	char	*expanded_filename;

	if (tokens[*i] == NULL || is_empty_or_whitespace(tokens[*i]))
		return (-1);
	type = get_redirection_type(tokens[*i]);
	if (type == NOT_REDIR)
		return (1);
	if (validate_redirection_syntax(tokens, i) == -1)
	{
		(*i)++;
		return (-1);
	}
	if (type == HEREDOC)
		return (parse_heredoc(shell, tokens, i, cmd));
	if (validate_and_expand(&expanded_filename, tokens[*i + 1], shell) == -1)
		return (-1);
	if (create_and_add_redirection(cmd, type, expanded_filename) == -1)
	{
		free(expanded_filename);
		return (-1);
	}
	free(expanded_filename);
	*i += 2;
	return (0);
}
