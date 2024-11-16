/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albelope <albelope@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/01 19:22:17 by pablogon          #+#    #+#             */
/*   Updated: 2024/11/16 21:23:09 by albelope         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <unistd.h>																		// write,open,read,close,access,unlink,getcwd,chdir, isatty, ttyname, ttyslot, ioctl, tcsetattr, tcgetattr
# include <stdio.h>																			//printf
# include <stdlib.h>																		//malloc, free, getenv
# include <stdbool.h>																		//true, false
# include <fcntl.h>  																		//open(), O_RDONLY, O_WRONLY...etc
# include <limits.h> 																		// INT_MAX, CHAR_MAX ...etc
# include <sys/types.h> 																	//fork()
# include <sys/wait.h> 																		//wait(), waitpid(), kill()
# include <sys/stat.h> 																		//stat,lstat, fstat
# include <signal.h> 																		//signal, sigaction, kill
# include <dirent.h> 																		//opendir, readdir, closedir
# include <term.h> 																			//tgetent, tgetflag, tgetnum, tgetstr, tgoto, tputs
# include <readline/readline.h> 															// readline, rl_clear_history, rl_on_new_line, rl_replace_line, rl_redisplay, add_history
# include <readline/history.h>

# include "../Libft/libft.h"
# include <limits.h>

//----------------- COLOR MACROS --------------------------//
# define WHITE		"\033[0m"																// Código de escape ANSI para color blanco.
# define BOLD		"\033[1m"																// Código de escape ANSI para texto en negrita.
# define GREEN		"\033[0;32m"															// Código de escape ANSI para color verde.
# define RED		"\033[0;31m"															// Código de escape ANSI para color rojo.
# define BLUE		"\033[0;34m"															// Código de escape ANSI para color azul.
# define YELLOW		"\033[33m"																// Código de escape ANSI para color amarillo.
# define MAGENTA	"\033[35m"																// Código de escape ANSI para color magenta.
# define CYAN		"\033[36m"																// Código de escape ANSI para color cian.

//----------------- GLOBAL VARIABLE -----------------------//

extern volatile sig_atomic_t	g_signal; 													// Variable global que se utiliza para manejar señales de forma segura.

//---------------------STRUCTURE --------------------------//

// Enumeración para definir códigos de error
typedef enum e_error 
{
    SUCCESS = 0,              	 									// Operación exitosa
    GENERAL_ERROR = 1,         										// Error general
    SYNTAX_ERROR = 2,          										// Error de sintaxis (quotes no balanceadas, pipes mal ubicados)
    MEMORY_ERROR = 3,          										// Error de memoria (malloc fallido)
    PERMISSION_DENIED = 126,   										// Permiso denegado al ejecutar un archivo
    CMD_NOT_FOUND = 127,       										// Comando no encontrado
    INVALID_EXIT = 128,        										// Argumento inválido para el comando exit
    EXIT_ERROR = 255           										// Error en el comando exit (fuera del rango permitido)
} t_error;

// Enumeración para definir tipos de tokens
typedef enum e_token
{
	CMD = 0,																				// Token para un comando
	AND = 1,																				// Token para el operador lógico AND
	OR = 2,																					// Token para el operador lógico OR
	PIPE = 3,																				// Token para un pipe (|)
	REDIR = 4,																				// Token para una redirección	
	OPEN_PAR = 5,																			// Token para un paréntesis abierto--SOLO PARA BONUS--
	CLOSE_PAR = 6,																			// Token para un paréntesis cerrado--SOLO PARA BONUS--
	END_OF_INPUT = 7,          															// Fin del input (EOF)																			
	UNKNOWN = -1,																			// Token desconocido
}	t_token;

// Enumeración para definir tipos de redirección
typedef enum e_type_redir
{
	INFILE = 10,																			// Redirección de archivo de entrada (<)
	APPEND = 11,																			// Redirección con append (>>)
	OUTFILE = 12,																			// Redirección de archivo de salida (>)
	HEREDOC = 13,																			// Redirección heredoc (<<)
	NOT_REDIR = -1,																			// No es una redirección
}	t_type_redir; 

// Estructura para manejar redirecciones
typedef struct s_redir
{
	enum e_type_redir		type;															// Tipo de redirección (<, >, >>, <<)
	char					*file;															// Ruta del archivo a redirigir
	int						fd;																// Descriptor de archivo
	struct s_redir			*next;															// Puntero al siguiente elemento en la lista
	struct s_redir			*prev;															// Puntero al elemento anterior en la lista
}	t_redir;

// Estructura para comandos y sus atributos
typedef struct s_cmd
{
	enum e_token	type;																	// Tipo de token (comando, operador, etc.)
	char			*path;																	// Ruta del comando ejecutable (ej: "/bin/ls")
	char			**arguments;															// Lista de argumentos del comando
	int				intfd;																	// Descriptor de archivo para entrada
	int				outfd;																	// Descriptor de archivo para salida
	int				pipe[2];																// Descriptor de archivo para pipes
	struct s_redir	*redir;																	// Puntero a la estructura de redirecciones
	struct s_cmd	*next;																	// Puntero al siguiente comando (si hay pipes)
	struct s_cmd	*prev;																	// Puntero al comando anterior (si hay pipes)
}	t_cmd;

// Estructura para las variables de entorno
typedef struct s_env
{
	char			*key;																	// Nombre de la variable de entorno (ej: "PATH")
	char			*value;																	// Valor de la variable de entorno
	struct s_env	*next;																	// Puntero al siguiente nodo de la lista de variables de entorno
	struct s_env	*prev;																	// Puntero al nodo anterior de la lista de variables de entorno
}	t_env;

// Estructura principal del minishell
typedef struct s_minishell
{
	int				exit_status;															// Último estado de salida de un comando ejecutado
	int				original_stdin;															// Descriptor de archivo original de la entrada estándar
	int				original_stdout;														// Descriptor de archivo original de la salida estándar
	int				running;																// Estado de ejecución del shell
	char			**env;																	// Copia del entorno del sistema
	struct s_env	*env_vars;																// Lista dinámica de variables de entorno
	struct s_cmd	*tokens;																// Lista de comandos o tokens actuales
}	t_minishell;

//------------------------------------------------------------------------------------------------------------------------------------------------------//

//--------------FUNCIONES PARSING---------------//
int         add_argument(char *token, int arg_index, t_cmd *cmd);                                 // Añade un argumento al comando actual
bool        contains_invalid_characters(char *input);                                             // Verifica si la entrada contiene caracteres inválidos
t_cmd       *create_new_command(t_minishell *shell);                                              // Crea una nueva estructura de comando
void        display_commands(t_cmd *cmd);                                                         // Muestra los comandos y argumentos para depuración
void        expand_tokens(t_cmd *cmd, t_minishell *shell);                                        // Expande las variables de entorno en los tokens
char        *expand_argument(const char *arg, t_minishell *shell);                                // Expande una variable de entorno en un argumento
char        *expand_entire_input(const char *input, t_minishell *shell);                          // Expande todas las variables de entorno en la entrada
char        *expand_env_vars(char *input, char **env);                                            // Gestiona la expansión de variables de entorno
char        *expand_string(const char *str, t_minishell *shell);                                  // Expande una cadena con variables de entorno
char        *expand_var_or_char(const char *str, int *i, t_minishell *shell);                     // Expande una variable o un carácter regular
char        *get_expanded_value(const char *variable, t_minishell *shell);                        // Obtiene el valor expandido de una variable
int         get_redirection_type(char *token);                                                    // Determina el tipo de redirección
t_token     get_special_token_type(char c);                                                       // Determina el tipo de token especial
int         handle_double_quotes(char *input, int i, char *buffer, int *buf_index);               // Maneja el contenido entre comillas dobles
int         handle_escape(char *input, int i, char *buffer, int *buf_index);                      // Maneja caracteres escapados (\)
char        *handle_escaped_dollar(int *i);                                                       // Maneja el escape de '$' en la expansión
int         handle_quotes(char *input, int i, char **tokens, int *j);                             // Maneja comillas (simples o dobles)
int         handle_single_quotes(char *input, int i, char *buffer, int *buf_index);               // Maneja el contenido entre comillas simples
char        *handle_dollar_sign(const char *str, int *i, t_minishell *shell);                     // Maneja el signo de dólar para expansión
char        *handle_regular_char(const char *str, int *i);                                        // Maneja caracteres regulares fuera de comillas
int         handle_special_char(char *input, int i, char **tokens, int *j);                       // Maneja caracteres especiales
int         handle_token(char *input, int i, char **tokens, int *j);                              // Procesa un token individual de la entrada
bool        is_quote(char c);                                                                     // Verifica si el carácter es una comilla (' o ")
int         is_empty_or_whitespace(char *str);                                                    // Verifica si una cadena está vacía o contiene solo espacios
int         is_special_char(char c);                                                              // Verifica si el carácter es un carácter especial
t_cmd       *initialize_first_command(t_minishell *shell);                                        // Inicializa el primer comando en la estructura
int         initialize_arguments(char **tokens, int *i, t_cmd *cmd);                              // Inicializa los argumentos del comando
t_cmd       *parse_input(char *input_line, t_minishell *shell);                                   // Parsea la línea de entrada y crea los comandos
void        print_command(t_cmd *cmd);                                                            // Imprime la estructura del comando para depuración
int         process_arguments(char **tokens, int *i, t_cmd *cmd, t_minishell *shell);             // Procesa los argumentos de los comandos
int         process_redirection(char **tokens, int *i, t_cmd *cmd, t_minishell *shell);           // Maneja las redirecciones (<, >, >>, <<)
int         process_token(char *input, int *i, char **tokens, int *j);                            // Procesa un token de la entrada
int         process_token_pipe(char **tokens, int *i, t_cmd **cmd, t_minishell *shell);           // Procesa los pipes (|)
int         process_tokens(char **tokens, t_cmd *current_cmd, t_minishell *shell);                // Procesa todos los tokens para construir los comandos
char        *read_input(void);                                                                    // Lee la entrada del usuario
char        *remove_quotes(const char *arg);                                                      // Elimina las comillas de un argumento
void        free_command(t_cmd *cmd);                                                             // Libera la memoria de la estructura de comando
void        free_command_list(t_cmd *cmd);                                                        // Libera la memoria de la lista de comandos
void        free_redirections(t_redir *redir);                                                    // Libera la memoria de las redirecciones
void        free_tokens_parse(char **tokens);                                                     // Libera la memoria de los tokens parseados
char        **tokenize_input(char *input);
void		replace_tabs_with_spaces(char *input);													// Reemplaza los tabuladores por espacios en blanco	
int         remove_extra_spaces(char *input, char *norm);
void 		print_error(const char *msg);
int 		print_error_and_return(const char *msg);
void 		print_error_and_exit(const char *msg, int exit_code);
int			error_handler(const char *msg, int exit_code);                                         // Elimina espacios en blanco adicionales de la entrada                       

//------------------------------------------------------------------------------------------------------------------------------------------------------//

//---------------------ENV-------------------------//
void		*safe_malloc(t_minishell *shell, size_t size);									// Asigna memoria de forma segura, manejando errores.
void		create_env_vars(t_minishell *shell, char **env);								// Crea una lista de variables de entorno desde el entorno del sistema.

//----------------------ERROR-----------------------//
void		fatal(int code, char *value);													// Maneja errores críticos que requieren la salida inmediata del shell.
void		ft_error(t_minishell *shell, int code, char * value, int should_exit);			// Maneja errores y opcionalmente termina la ejecución.

//-----------------FREE_UTILS---------------------//
void		free_array(char ***array);														// Libera la memoria de un array de strings.
void		free_redir(t_redir **redir);													// Libera la memoria asociada a las redirecciones.
void		free_env_list(t_env	**l_env);													// Libera la memoria de la lista de variables de entorno.
void		*free_shell(t_minishell **shell);												// Libera toda la memoria del shell.
void		*free_tokens(t_cmd **tokens);													// Libera la memoria asociada a los tokens.

//-------------FUNCIONES MAIN------------------//
void		exit_shell(t_minishell *shell);													// Función para salir del shell limpiamente.
void		init_minishell(t_minishell *shell, char **env);									// Inicializa el shell con las variables de entorno.

//-----------------SIGNALS---------------------//
void		signal_handler(int signal);														//Gestion de señales (Ctrl+C , Ctrl + D, Ctrl + \)

//-------------------UTILS---------------------//
char		*ft_strncpy(char *dest, const char *src, size_t n);								// Copia hasta n caracteres de la cadena src a dest
char		*ft_strndup(const char *s, size_t n);											// Duplica hasta n caracteres de la cadena s y devuelve un nuevo puntero a la copia.

//------------------------------------------------------------------------------------------------------------------------------------------------------//

//--------------FUNCIONES EXECUTION-------------//
int			execute_command(t_minishell *shell);											// Ejecuta un comando individual.
int			handle_builtin(t_cmd *cmd, t_minishell *shell);									//Maneja comandos internos (built-ins)
void		handle_pipes(t_cmd *cmd, t_minishell *shell);									// Maneja la ejecución de comandos conectados por pipes.
void		handle_redirection(t_cmd *cmd);													// Maneja las redirecciones de entrada y salida.

//---------------------UTILS-------------------//
char		*get_command_path(char *cmd, t_minishell *shell);								//Obtiene la ruta del comando proporcionado

//------------------BUILTIN-CD---------------//
char		*get_env_value(t_env *env_list, char *key);										//Obtiene el valor de una variable de entorno
void		update_env_var(t_env **env_list, char *key, char *value);						//Actualiza el valor de una variable de entorno
void		ft_cd(t_minishell *shell, char **arg);											//Cambia el directorio de trabajo actual
//------------------BUILTIN-ECHO---------------//
void		ft_echo(t_minishell *shell, char **arg);										//Imprime argumentos en la salida estándar
//------------------BUILTIN-PWD----------------//
void		ft_pwd(t_minishell *shell);														//Imprime el directorio de trabajo actual
//------------------BUILTIN-EXPORT-------------//
int			is_valid_identifier(const char *str);											//Verifica si la cadena es un identificador válido para variables de entorno
void		print_env_vars(t_env *env_list);												//Imprime todas las variables de entorno
void		ft_export(t_minishell *shell, char **arg);										//Añade o actualiza variables de entorno
//------------------BUILTIN-UNSET--------------//
void		delete_env_var(t_env **env_list, const char *key);								//Elimina una variable de entorno
void		ft_unset(t_minishell *shell, char **arg);										//Elimina variables de entorno específicas
//------------------BUILTIN-ENV----------------//
void		ft_env(t_minishell *shell);														//Imprime las variables de entorno actuales


#endif
