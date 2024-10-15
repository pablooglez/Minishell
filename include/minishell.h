/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pabloglez <pabloglez@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/01 19:22:17 by pablogon          #+#    #+#             */
/*   Updated: 2024/10/15 20:47:56 by pabloglez        ###   ########.fr       */
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

//----------------- COLOR MACROS --------------------------//
# define WHITE		"\033[0m"									// Código de escape ANSI para color blanco.
# define BOLD		"\033[1m"									// Código de escape ANSI para texto en negrita.
# define GREEN		"\033[0;32m"								// Código de escape ANSI para color verde.
# define RED		"\033[0;31m"								// Código de escape ANSI para color rojo.
# define BLUE		"\033[0;34m"								// Código de escape ANSI para color azul.
# define YELLOW		"\033[33m"									// Código de escape ANSI para color amarillo.
# define MAGENTA	"\033[35m"									// Código de escape ANSI para color magenta.
# define CYAN		"\033[36m"									// Código de escape ANSI para color cian.

//----------------- GLOBAL VARIABLE -----------------------//

extern volatile sig_atomic_t	g_signal; 						// Variable global que se utiliza para manejar señales de forma segura.

//---------------------STRUCTURE --------------------------//

// Enumeración para definir códigos de error
typedef enum e_error 
{
	MEMORY = 20,												// Error de memoria
	CMD_NOT_FOUND = 21											// Error de comando no encontrado
}	t_error;

// Enumeración para definir tipos de tokens
typedef enum e_token
{
	CMD = 0,													// Token para un comando
	AND = 1,													// Token para el operador lógico AND
	OR = 2,														// Token para el operador lógico OR
	PIPE = 3,													// Token para un pipe (|)
	OPEN_PAR = 5,												// Token para un paréntesis abierto
	CLOSE_PAR = 6,												// Token para un paréntesis cerrado
	REDIR = 7,													// Token para redirección
	UNKNOWN = -1,												// Token desconocido
}	t_token;

// Enumeración para definir tipos de redirección
typedef enum e_type_redir
{
	INFILE = 10,												// Redirección de archivo de entrada (<)
	APPEND = 11,												// Redirección con append (>>)
	OUTFILE = 12,												// Redirección de archivo de salida (>)
	HEREDOC = 13,												// Redirección heredoc (<<)
	NOT_REDIR = -1,												// No es una redirección
}	t_type_redir;

// Estructura para manejar redirecciones
typedef struct s_redir
{
	enum e_type_redir		type;								// Tipo de redirección (<, >, >>, <<)
	char					*file;								// Ruta del archivo a redirigir
	int						fd;									// Descriptor de archivo
	struct s_redir			*next;								// Puntero al siguiente elemento en la lista
	struct s_redir			*prev;								// Puntero al elemento anterior en la lista
}	t_redir;

// Estructura para comandos y sus atributos
typedef struct s_cmd
{
	enum e_token	type;										// Tipo de token (comando, operador, etc.)
	char			*path;										// Ruta del comando ejecutable (ej: "/bin/ls")
	char			**arguments;								// Lista de argumentos del comando
	int				intfd;										// Descriptor de archivo para entrada
	int				outfd;										// Descriptor de archivo para salida
	int				pipe[2];									// Descriptor de archivo para pipes
	struct s_redir	*redir;										// Puntero a la estructura de redirecciones
	struct s_cmd	*next;										// Puntero al siguiente comando (si hay pipes)
	struct s_cmd	*prev;										// Puntero al comando anterior (si hay pipes)
}	t_cmd;

// Estructura para las variables de entorno
typedef struct s_env
{
	char			*key;										// Nombre de la variable de entorno (ej: "PATH")
	char			*value;										// Valor de la variable de entorno
	struct s_env	*next;										// Puntero al siguiente nodo de la lista de variables de entorno
	struct s_env	*prev;										// Puntero al nodo anterior de la lista de variables de entorno
}	t_env;

// Estructura principal del minishell
typedef struct s_minishell
{
	int				exit_status;								// Último estado de salida de un comando ejecutado
	int				original_stdin;								// Descriptor de archivo original de la entrada estándar
	int				original_stdout;							// Descriptor de archivo original de la salida estándar
	int				running;									// Estado de ejecución del shell
	char			**env;										// Copia del entorno del sistema
	struct s_env	*env_vars;									// Lista dinámica de variables de entorno
	struct s_cmd	*tokens;									// Lista de comandos o tokens actuales
}	t_minishell;

//-------------FUNCIONES MAIN------------------//
void		exit_shell(t_minishell *shell);						// Función para salir del shell limpiamente.
void		init_minishell(t_minishell *shell, char **env);		// Inicializa el shell con las variables de entorno.
int			add_history(const char *);							//Añade un comando al historial

//---------------------ENV-------------------------//
void		*safe_malloc(t_minishell *shell, size_t size);		// Asigna memoria de forma segura, manejando errores.
void		create_env_vars(t_minishell *shell, char **env);	// Crea una lista de variables de entorno desde el entorno del sistema.

//----------------------ERROR-----------------------//
void		fatal(int code, char *value);						// Maneja errores críticos que requieren la salida inmediata del shell.
void		ft_error(t_minishell *shell, int code, char * value, int should_exit); // Maneja errores y opcionalmente termina la ejecución.

//-----------------FREE_UTILS---------------------//
void		*free_tokens(t_cmd **tokens);						// Libera la memoria asociada a los tokens.
void		free_array(char ***array);							// Libera la memoria de un array de strings.
void		free_redir(t_redir **redir);						// Libera la memoria asociada a las redirecciones.
void		*free_shell(t_minishell **shell);					// Libera toda la memoria del shell.
void		free_env_list(t_env	**l_env);						// Libera la memoria de la lista de variables de entorno.

//-------------------UTILS---------------------//
char		*ft_strncpy(char *dest, const char *src, size_t n);
char		*ft_strndup(const char *s, size_t n);

//--------------FUNCIONES PARSING---------------//
t_cmd		*parse_input(char *input_line, t_minishell *shell); // Función para separar la línea de entrada en tokens (comandos y operadores).

//--------------FUNCIONES EXECUTION-------------//
int			heardoc(t_minishell *shell);						// Implementación del heredoc para redirección.
int			execute_command(t_minishell *shell);				// Ejecuta un comando individual.
int			handle_builtin(t_cmd *cmd, t_minishell *shell);		// Maneja la ejecución de comandos internos (builtins).
void		handle_redirection(t_cmd *cmd);						// Maneja las redirecciones de entrada y salida.
void		handle_pipes(t_cmd *cmd);							// Maneja la ejecución de comandos conectados por pipes.
void		signal_handler(int signal);							// Función para manejar señales del sistema como Ctrl+C o Ctrl+D.

//--------------BUILT-INS----------------------//
//------------------CD---------------//
char		*get_env_value(t_env *env_list, char *key);
void		update_env_var(t_env **env_list, char *key, char *value);
void		ft_cd(t_minishell *shell, char **arg);
//------------------ECHO---------------//
void		ft_echo(t_minishell *shell, char **arg);
//------------------PWD----------------//
void		ft_pwd(t_minishell *shell);
//------------------EXPORT-------------//
void		ft_export(t_minishell *shell, char **arg);
//------------------UNSET--------------//
void		ft_unset(t_minishell *shell, char **arg);
//------------------ENV----------------//
void		ft_env(t_minishell *shell);
//------------------EXIT---------------//
void		ft_exit(t_minishell *shell);

#endif
