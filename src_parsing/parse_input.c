/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_input.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albelope <albelope@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/11 21:23:53 by albelope          #+#    #+#             */
/*   Updated: 2024/11/01 21:27:09 by albelope         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"


t_cmd	*initialize_first_command(t_minishell *shell)
{
	t_cmd	*cmd;

	cmd = create_new_command(shell);
	if (!cmd)
		return (NULL);
	return (cmd);
}

int	initialize_arguments(char **tokens, int *i, t_cmd *cmd)
{
	if (!cmd->path)
	{
		cmd->path = ft_strdup(tokens[*i]);
		if (!cmd->path)
			return (-1);
		(*i)++;
	}
	cmd->arguments = ft_calloc(100, sizeof(char *));
	if (!cmd->arguments)
		return (-1);
	return (0);
}

int	add_argument(char *token, int arg_index, t_cmd *cmd)
{
	cmd->arguments[arg_index] = ft_strdup(token);
	if (!cmd->arguments[arg_index])
		return (-1);
	return (0);
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
int	process_arguments(char **tokens, int *i, t_cmd *cmd, t_minishell *shell)
{
    int	arg_index;
    int	ret;

    if (initialize_arguments(tokens, i, cmd) == -1)
        return (-1);
    arg_index = 0;
    cmd->arguments[arg_index] = ft_strdup(cmd->path);
    if (!cmd->arguments[arg_index])
        return (-1);
    
    arg_index++;
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
}

int	process_tokens(char **tokens, t_cmd *current_cmd, t_minishell *shell)
{
	int	i;

	i = 0;
	while (tokens[i])
	{
		if (process_token_pipe(tokens, &i, &current_cmd, shell) == -1)
			return (-1);
		if (process_arguments(tokens, &i, current_cmd, shell) == -1)
			return (-1);
	}
	return (0);
}

t_cmd	*parse_input(char *input_line, t_minishell *shell)
{
	char	**tokens;
	t_cmd	*cmd;

	if (is_empty_or_whitespace(input_line))
        return (NULL);
	if (contains_invalid_characters(input_line))
		return (NULL);
	tokens = tokenize_input(input_line);
	if (!tokens || !tokens[0])
	{
		free_tokens_parse(tokens);
		return (NULL);
	}
	cmd = initialize_first_command(shell);
	if (!cmd)
	{
		free_tokens_parse(tokens);
		return (NULL);
	}
	if (tokens[0][0] == '$')
		cmd->path = ft_strdup("echo");
	if (process_tokens(tokens, cmd, shell) == -1)
	{
		free_tokens_parse(tokens);
		free_command(cmd);
		return (NULL);
	}
	expand_tokens(cmd, shell);
	free_tokens_parse(tokens);
	//print_command(cmd);
	return (cmd);
}
