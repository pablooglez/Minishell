/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pablogon <pablogon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/01 19:22:17 by pablogon          #+#    #+#             */
/*   Updated: 2024/10/03 14:10:17 by pablogon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <unistd.h> /*write,open,read,close,access,unlink,getcwd,chdir, isatty, ttyname, ttyslot, ioctl, tcsetattr, tcgetattr */
# include <stdio.h> //printf
# include <stdlib.h> //malloc, free, getenv
# include <stdbool.h> //true, false
# include <fcntl.h>  //open(), O_RDONLY, O_WRONLY...etc
# include <limits.h> // INT_MAX, CHAR_MAX ...etc
# include <sys/types.h> //fork()
# include <sys/wait.h> //wait(), waitpid(), kill()
# include <sys/stat.h> //stat,lstat, fstat
# include <signal.h> //signal, sigaction, kill
# include <dirent.h> //opendir, readdir, closedir
# include <term.h> //tgetent, tgetflag, tgetnum, tgetstr, tgoto, tputs
# include <readline/readline.h> /* readline, rl_clear_history, rl_on_new_line, rl_replace_line, rl_redisplay, add_history */
# include <readline/history.h>
# include "../Libft/libft.h"

# define PROMPT "minishell> "

typedef struct s_minishell
{
	char			**env;
	int				last_exit_status;
	int				running; //Control de estado de la shell
	char			*history_file; //Archivo para almacenar el historial de comandos (opcional)
	struct s_env	*env_vars //Lista dinámica de variables de entorno
}	t_minishell;

typedef struct s_command
{
	char	*command; //Comando principal (ejemplo: "ls")
	char	**arguments; // Argumentos del comando (ejemplo: ["-l"])
	int		is_pipe; //Indica si el comando esta conectadoa una pipe
	char	*input_file; //Gestionar redirecciones de entrada
	char	*output_file; //Gestionar redirecciones de salida
	int		append; // 1 si se usa '>>' o 0 si '>'
	int		background; // 1 si el comando debe ejecutarse en segundo plano ('&')
	int		redirection_error; //Código error para redirecciones (Ejemplo: Permisos denegados)
	struct s_command	*next; //Siguiente comando en las listas (Para Pipes)
}	t_command;




//--------------FUNCIONES PARSING---------------//
char		*read_input(void); //Lee la entrada del usuario
t_command	*parse_input(char *input_line); //Separa la entrada en tokens y crea el comando (parsing)
char		*expand_env_vars(char *input, char **env); //Gestiona la expansion de variables de entorno($USER, $?)

//--------------FUNCIONES EXECUTION------------//
int			execute_command(t_command *cmd, t_minishell *shell); //Ejecuta un comando individual
int			handle_builtin(t_command *cmd, t_minishell *shell); //Maneja comandos internos (built-ins)
void		handle_redirection(t_command *cmd); // Maneja redirecciones de entrada y de salida
void		handle_pipes(t_command *cmd); // Maneja pipes entre comandos
void		signal_handler(int signal); //Gestion de señales (Ctrl+C , Ctrl + D, Ctrl + \)

//-------------FUNCIONES MAIN------------------//
void		free_command(t_command *cmd); // Libera memoria asignada a un comando
void		cleanup_minishell(t_minishell *shell); // Liberar recursos minishell







#endif