/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pabloglez <pabloglez@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/28 01:10:19 by pablogon          #+#    #+#             */
/*   Updated: 2024/11/29 13:53:05 by pabloglez        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	sigint_heredoc(int sig)
{
	(void) sig;
	g_signal = 2;

	ioctl(STDIN_FILENO, TIOCSTI, "\n");
}

static	void	read_heredoc(int fd, char *delimiter)
{
	char	*line;

	signal(SIGINT, sigint_heredoc);
	line = NULL;
	line = readline("> ");
	while (line && ft_strcmp(delimiter, line) && g_signal == 0)
	{
		write(fd, line, ft_strlen(line));
		write(fd, "\n", 1);
		free(line);
		line = readline("> ");
	}
	close(fd);
	if (line)
		free(line);
	free(delimiter);
	signal(SIGINT, signal_handler);
}

static int	create_heredoc(t_redir *redir)
{
	int					fd;
	static int			unique;
	char				*st_unique;
	char				*delimiter;

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
			return (0);
		redir->type = INFILE;
		read_heredoc(fd, delimiter);
	}
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
	return (create_heredoc(cmd->redir));
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
