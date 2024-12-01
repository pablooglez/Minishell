/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albelope <albelope@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/01 19:22:17 by pablogon          #+#    #+#             */
/*   Updated: 2024/12/01 13:15:57 by albelope         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <stdbool.h>
# include <fcntl.h>
# include <linux/limits.h>
# include <limits.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <sys/stat.h>
# include <signal.h>
# include <dirent.h>
# include <term.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <errno.h>
# include <sys/ioctl.h>
# include "../Libft/libft.h"

//----------------- COLOR MACROS --------------------------//
# define WHITE		"\033[0m"
# define BOLD		"\033[1m"
# define GREEN		"\033[0;32m"
# define RED		"\033[0;31m"
# define BLUE		"\033[0;34m"
# define YELLOW		"\033[33m"
# define MAGENTA	"\033[35m"
# define CYAN		"\033[36m"

//----------------- GLOBAL VARIABLE -----------------------//

extern int	g_signal;

//---------------------STRUCTURE --------------------------//

typedef enum e_error
{
	SUCCESS = 0,
	GENERAL_ERROR = 1,
	SYNTAX_ERROR = 2,
	MEMORY_ERROR = 3,
	PERMISSION_DENIED = 126,
	CMD_NOT_FOUND = 127,
	INVALID_EXIT = 128,
	EXIT_ERROR = 255,
	FD_NOT_FOUND = 129,
	CD_NOT_FOUND = 24,
	MSG = 100,
}	t_error;

typedef enum e_token
{
	CMD = 0,
	AND = 1,
	OR = 2,
	PIPE = 3,
	REDIR = 4,
	OPEN_PAR = 5,
	CLOSE_PAR = 6,
	END_OF_INPUT = 7,
	UNKNOWN = -1,
}	t_token;

typedef enum e_type_redir
{
	INFILE = 10,
	APPEND = 11,
	OUTFILE = 12,
	HEREDOC = 13,
	NOT_REDIR = -1,
}	t_type_redir;

typedef struct s_redir
{
	enum e_type_redir		type;
	char					*file;
	int						fd;
	struct s_redir			*next;
	struct s_redir			*prev;
}	t_redir;

typedef struct s_cmd
{
	enum e_token	type;
	char			*path;
	char			**arguments;
	int				intfd;
	int				outfd;
	int				pid;
	int				pipe[2];
	struct s_redir	*redir;
	struct s_cmd	*next;
	struct s_cmd	*prev;
}	t_cmd;

typedef struct s_env
{
	char			*key;
	char			*value;
	struct s_env	*next;
	struct s_env	*prev;
}	t_env;

typedef struct s_minishell
{
	int				exit_status;
	int				original_stdin;
	int				original_stdout;
	bool			is_child;
	char			**env;
	struct s_env	*env_vars;
	struct s_cmd	*tokens;
	char			*input;
	int				i;
	int				j;
	int				e;
}	t_minishell;

//----------------------------------------------------------------------------//

//--------------EXPANSION FUNCTIONS---------------//
void		expand_tokens(t_cmd *cmd, t_minishell *shell);
char		*expand_argument(const char *arg, t_minishell *shell);
char		*expand_entire_input(const char *input, t_minishell *shell);
char		*expand_env_vars(char *input, char **env);
char		*expand_string(const char *str, t_minishell *shell);
char		*expand_var_or_char(const char *str, int *i, t_minishell *shell);
char		*get_expanded_value(const char *variable, t_minishell *shell);
int			expand_variable(char *input, char *buffer, int *buf_index,
				t_minishell *shell);

//--------------TOKEN FUNCTIONS---------------//
int			handle_token(char **tokens, t_minishell *shell);
t_token		get_special_token_type(char c);
int			process_token(char *input, char **tokens, int *j,
				t_minishell *shell);
int			process_token_pipe(char **tokens, int *i, t_cmd **cmd,
				t_minishell *shell);
int			process_tokens(char **tokens, t_cmd *current_cmd,
				t_minishell *shell);
t_token		classify_special_token(char c);
int			handle_special_token_cases(char **tokens, char *buffer,
				int *buf_index,
				t_minishell *shell);
int			spec(char **tokens, char *buffer, int *buf_index,
				t_minishell *shell);
int			process_input_character(char **tokens, char *buffer, int *buf_index,
				t_minishell *shell);
char		**tokenize_input(char *input, t_minishell *shell);

//--------------COMMAND AND REDIRECTION FUNCTIONS---------------//
t_cmd		*create_new_command(t_minishell *shell);
int			add_argument(char *token, int arg_index, t_cmd *cmd);
int			initialize_arguments(char **tokens, int *i, t_cmd *cmd);
int			process_arguments(char **tokens, int *i, t_cmd *cmd,
				t_minishell *shell);
int			process_redirection(char **tokens, int *i, t_cmd *cmd,
				t_minishell *shell);
int			get_redirection_type(char *token);
void		free_command(t_cmd *cmd);
void		free_command_list(t_cmd *cmd);
void		free_redirections(t_redir *redir);
void		free_tokens_parse(char **tokens);

//--------------ERROR FUNCTIONS---------------//
void		print_error(const char *msg);
int			print_error_and_return(const char *msg);
void		print_error_and_exit(const char *msg, int exit_code);
int			error_handler(const char *msg, int exit_code);

//--------------UTILITY FUNCTIONS---------------//
int			contains_invalid_characters(char *input);
int			is_empty_or_whitespace(char *str);
int			handle_escape(char *input, int i, char *buffer, int *buf_index);
char		*handle_escaped_dollar(int *i);
char		*handle_dollar_sign(const char *str, int *i, t_minishell *shell);
char		*handle_regular_char(const char *str, int *i);
int			handle_special_char(char *input, int i, char **tokens, int *j);
int			handle_dollar_case(char *buffer, int *buf_index,
				t_minishell *shell);
int			get_variable_name(char *input, char *var_name, int *var_len,
				t_minishell *shell);
int			copy_variable_value(char *var_name, char *buffer, int *buf_index,
				t_minishell *shell);
char		*remove_quotes(const char *arg);
int			heredoc_quoted(const char *str);
t_cmd		*parse_input(char *input_line, t_minishell *shell);
void		print_command(t_cmd *cmd);

//-----------------HEREDOC---------------------//
int			parse_heredoc(t_minishell *shell,
				char **tokens, int *i, t_cmd *cmd);
void		delete_heredoc(t_minishell *shell);

//---------------------------------------------------------------------------//

//---------------------ENV_VARS-------------------------//
void		*safe_malloc(t_minishell *shell, size_t size);
void		create_env_vars(t_minishell *shell, char **env);

//----------------------ERROR-----------------------//
int			ft_error(t_minishell *shell, int code,
				char *value, int should_exit);

//-----------------FREE_UTILS---------------------//
void		free_array(char ***array);
void		*free_shell(t_minishell **shell);
void		*free_tokens(t_cmd **tokens);

//-----------------SIGNALS---------------------//
void		signal_handler(int signal);
int			event_hook_readline(void);
//-------------------UTILS---------------------//
char		*ft_strncpy(char *dest, const char *src, size_t n);
char		*ft_strndup(const char *s, size_t n);

//-------------------ENV_UTILS-------------//
char		**env_vars_to_array(t_env *env_vars);

//--------------EXECUTE-------------//
void		execute(t_minishell *shell);

//--------------REDIRECTIONS-------------//
int			handle_pipe(t_cmd *cmd);
int			handle_redirection(t_minishell *shell, t_redir *redir, int fd);
void		safe_dup2(int fd1, int fd2);
void		safe_close(int fd);

//---------------------UTILS-------------------//
char		*check_absolute_or_relative_path(char *cmd);
char		*get_env_value(t_env *env_list, char *key);
void		update_env_var(t_env **env_list, char *key, char *value);
int			is_valid_identifier(const char *str);

//---------------------UTILS2-------------------//
char		*get_command_path(char *cmd, t_minishell *shell);

//------------------BUILTIN-INS---------------//
int			handle_builtin(t_cmd *cmd, t_minishell *shell);

//------------------BUILTIN-CD---------------//
int			ft_cd(t_minishell *shell, char **arg);

//------------------BUILTIN-ECHO---------------//
void		ft_echo(t_minishell *shell, char **arg);

//------------------BUILTIN-ENV----------------//
void		ft_env(t_minishell *shell);

//------------------BUILTIN-EXIT---------------//
void		ft_exit(t_minishell *shell, char **arg);

//------------------BUILTIN-EXPORT-------------//
int			ft_export(t_minishell *shell, char **arg);

//------------------BUILTIN-PWD----------------//
int			ft_pwd(t_minishell *shell);

//------------------BUILTIN-UNSET--------------//
void		ft_unset(t_minishell *shell, char **arg);
#endif
