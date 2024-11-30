/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/28 01:10:19 by pablogon          #+#    #+#             */
/*   Updated: 2024/11/30 21:07:40 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	sigint_heredoc(int sig)
{
	g_signal = sig;
	rl_done = 1;
}

static void	read_heredoc(int fd, char *delimiter, t_minishell *shell)
{
	char	*line;
	char	*expanded_line;
	int		expand;

	expand = !heredoc_quoted(delimiter);
	delimiter = ft_strtrim(delimiter, "'\"");
	signal(SIGINT, sigint_heredoc);
	line = readline("> ");
	while (line && ft_strcmp(delimiter, line) && g_signal == 0)
	{
		if (expand)
			expanded_line = expand_string(line, shell);
		else
			expanded_line = ft_strdup(line);
		write(fd, expanded_line, ft_strlen(expanded_line));
		write(fd, "\n", 1);
		free(line);
		free(expanded_line);
		line = readline("> ");
	}
	close(fd);
	if (line)
		free(line);
	free(delimiter);
	signal(SIGINT, signal_handler);
}

static int	create_heredoc(t_redir *redir, t_minishell *shell)
{
	int			fd;
	static int	unique;
	char		*st_unique;
	char		*delimiter;

	fd = -1;
	while (redir && redir->next)
		redir = redir->next;
	delimiter = redir->file;
	if (redir && redir->type == HEREDOC && delimiter)
	{
		st_unique = ft_itoa(unique++);
		redir->file = ft_strjoin("/tmp/hd_", st_unique);
		free(st_unique);
		fd = open(redir->file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if (fd == -1)
		{
			free(delimiter);
			return (0);
		}
		redir->type = INFILE;
		read_heredoc(fd, delimiter, shell);
	}
	free(delimiter);
	return ((g_signal != 0) * -1);
}

int	parse_heredoc(t_minishell *shell, char **tokens, int *i, t_cmd *cmd)
{
	t_redir	*new_redir;
	t_redir	*current;

	new_redir = malloc(sizeof(t_redir));
	if (!new_redir)
		ft_error(shell, MEMORY_ERROR, NULL, 1);
	new_redir->next = NULL;
	new_redir->type = HEREDOC;
	new_redir->file = ft_strdup(tokens[*i + 1]);
	if (!new_redir->file)
	{
		free(new_redir);
		ft_error(shell, MEMORY_ERROR, NULL, 1);
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
	*i += 2;
	return (create_heredoc(cmd->redir, shell));
}

void	delete_heredoc(t_minishell *shell)
{
	t_cmd	*cmd;
	t_redir	*redir;

	cmd = shell->tokens;
	while (cmd)
	{
		redir = cmd->redir;
		while (redir)
		{
			if (!ft_strcmp(redir->file, "/tmp/hd_"))
				unlink (redir->file);
			redir = redir->next;
		}
		cmd = cmd->next;
	}
}
