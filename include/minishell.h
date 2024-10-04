/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pablogon <pablogon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/01 19:22:17 by pablogon          #+#    #+#             */
/*   Updated: 2024/10/04 19:57:41 by pablogon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <unistd.h>											// write,open,read,close,access,unlink,getcwd,chdir, isatty, ttyname, ttyslot, ioctl, tcsetattr, tcgetattr
# include <stdio.h>												//printf
# include <stdlib.h>											//malloc, free, getenv
# include <stdbool.h>											//true, false
# include <fcntl.h>  											//open(), O_RDONLY, O_WRONLY...etc
# include <limits.h> 											// INT_MAX, CHAR_MAX ...etc
# include <sys/types.h> 										//fork()
# include <sys/wait.h> 											//wait(), waitpid(), kill()
# include <sys/stat.h> 											//stat,lstat, fstat
# include <signal.h> 											//signal, sigaction, kill
# include <dirent.h> 											//opendir, readdir, closedir
# include <term.h> 												//tgetent, tgetflag, tgetnum, tgetstr, tgoto, tputs
# include <readline/readline.h> 								// readline, rl_clear_history, rl_on_new_line, rl_replace_line, rl_redisplay, add_history
# include <readline/history.h>
# include "../Libft/libft.h"

# define WHITE		"\033[0m"
# define BOLD		"\033[1m"
# define GREEN		"\033[0;32m"
# define RED		"\033[0;31m"
# define BLUE		"\033[0;34m"
# define YELLOW		"\033[33m"
# define MAGENTA	"\033[35m"
# define CYAN		"\033[36m"

enum e_error {
	MEMORY = 20,
	CMD_NOT_FOUND = 21
};

typedef struct s_redir
{
	int		type;												//Tipo de redirreción (<, >, >>)
	char	*ruta;												//Ruta del archivo para la redireccion
}	t_redir;

typedef struct s_cmd
{
	char	*path;												//Comando principal (ejemplo: "/bin/ls")
	char	**arguments;										// Argumentos del comando (ejemplo:[ls] ["-l"])
	int		pipe[2];											//Descriptor de archivo para el pipe
	t_redir	*redir;												//Redirecciones de entrada y de salida
	struct s_cmd	*next;										//Siguiente comando en las listas (para pipes)
	struct s_cmd	*prev;										//Comando anterior en la lista (para pipes)
}	t_cmd;

typedef struct s_env
{
	char			*key;										//Nombre de la variable de entorno (ejemplo: "PATH")
	char			*value;										//Valor de la variable de entorno
	struct s_env	*next;										//Puntero al siguiente nodo de la lista
	struct s_env	*prev;										//Puntero al anterior nodo de la lista
}	t_env;

typedef struct s_minishell
{
	int				exit_status;								//Ultimo estado de la salida de un comando ($?)
	int				running;									//Control de estado de la shell (1 si se esta ejecutando)
	t_env			*env_vars;									//Lista dinámica de variables de entorno
	t_cmd			*cmd;										//Comando actual o lista de comandos
}	t_minishell;

//--------------FUNCIONES PARSING---------------//
void		read_input(void);									//Lee la entrada del usuario
t_cmd		*parse_input(char *input_line);						//Separa la entrada en tokens y crea el comando (parsing)
char		*expand_env_vars(char *input, char **env);			//Gestiona la expansion de variables de entorno($USER, $?)

//--------------FUNCIONES EXECUTION------------//
int			heardoc(t_minishell *shell);
int			execute_command(t_cmd *cmd, t_minishell *shell);	//Ejecuta un comando individual
int			handle_builtin(t_cmd *cmd, t_minishell *shell);		//Maneja comandos internos (built-ins)
void		handle_redirection(t_cmd *cmd);						//Maneja redirecciones de entrada y de salida
void		handle_pipes(t_cmd *cmd);							//Maneja pipes entre comandos
void		signal_handler(int signal);							//Gestion de señales (Ctrl+C , Ctrl + D, Ctrl + \)

//-------------FUNCIONES MAIN------------------//
void		free_command(t_cmd *cmd);							// Libera memoria asignada a un comando
void		cleanup_minishell(t_minishell *shell);				// Liberar recursos minishell

//-----------------ERROR-----------------------//
void		ft_error(t_minishell *shell, int code, char * value, int should_exit);




#endif