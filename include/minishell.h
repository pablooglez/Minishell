/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pablogon <pablogon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/01 19:22:17 by pablogon          #+#    #+#             */
/*   Updated: 2024/10/02 21:24:37 by pablogon         ###   ########.fr       */
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












#endif