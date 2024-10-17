# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: albelope <albelope@student.42malaga.com    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/10/01 19:11:08 by pablogon          #+#    #+#              #
#    Updated: 2024/10/17 13:14:03 by albelope         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #


################################################################################
##                              COMPILATION INFO                              ##
################################################################################

NAME:= minishell

CFLAGS:= -g -Wall -Werror -Wextra \
		#-g -fsanitize=address,undefined \
		#-Wunreachable-code -Ofast \

MINISHELL = include/
LIBFT_DIR = ./Libft
LIBFT = $(LIBFT_DIR)/libft.a

HEADERS := -I include -I$(LIBFT_DIR) ## CAMBIADO A LIBFT_DIR PARA QUE COJA LA LIBRERIA LIBFT##

################################################################################
##                              SOURCE AND OBJECTS                            ##
################################################################################

SRCS	:= src/env.c \
			src/error.c \
			src/free_utils.c \
			src/main.c \
			src/utils.c \


SRCS_EXEC = src_exec/built-ins/built-ins.c\
			src_exec/built-ins/cd.c \
			src_exec/built-ins/echo.c \
			src_exec/built-ins/env.c \
			src_exec/execute.c \

SRCS_PARSER = src_parsing/read_input.c src_parsing/tokenize.c \
				src_parsing/parse_input.c src_parsing/handle_quotes.c \
				src_parsing/handle_char.c src_parsing/handle_pipes.c \
				src_parsing/command_utils.c src_parsing/code_checks.c \
				src_parsing/handle_redirections.c

OBJS		:= ${SRCS:.c=.o}
OBJS_PARSER	:= ${SRCS_PARSER:.c=.o}
OBJS_EXEC	:= ${SRCS_EXEC:.c=.o}

################################################################################
##                                   COLORS                                   ##
################################################################################

END=\033[0m
BOLD=\033[1m
RED=\033[0;31m
GREEN=\033[0;32m
YELLOW=\033[33m
BLUE=\033[0;34m
MAGENTA=\033[35m
CYAN=\033[36m
WHITE=\033[37m

################################################################################
##                                   RULES                                    ##
################################################################################

all: libft $(NAME)

head:
	@echo "$(GREEN)$(BOLD)"
	@echo "\t███    ███ ██ ███    ██ ██ ███████ ██   ██ ███████ ██      ██      "
	@echo "\t████  ████ ██ ████   ██ ██ ██      ██   ██ ██      ██      ██      "
	@echo "\t██ ████ ██ ██ ██ ██  ██ ██ ███████ ███████ █████   ██      ██      "
	@echo "\t██  ██  ██ ██ ██  ██ ██ ██      ██ ██   ██ ██      ██      ██      "
	@echo "\t██      ██ ██ ██   ████ ██ ███████ ██   ██ ███████ ███████ ███████ "
	@echo "$(END)"
	@echo "$(BOLD)\t        42MLG: by pablogon && albelope$(END)"
	@echo "\tProyect: $(CYAN)minishell$(END)"
	@echo "\tCommands: $(CYAN)all clean fclean re $(END)"
	@echo "$(BLUE)\t🛠   Compiler: $(BOLD)$(CC)$(END)\n"


libft:
	@make -s -C $(LIBFT_DIR)

$(NAME): line $(OBJS) $(OBJS_PARSER) $(OBJS_EXEC)
	@echo "✦ ---------------------- ✦$(END)"
	@$(CC) $(CFLAGS) $(OBJS) $(OBJS_PARSER) $(OBJS_EXEC) $(LIBFT) $(HEADERS) $(LINK) -lreadline -o $(NAME)
####AÑADIDO -LREADLINE PARA QUE FUNCIONE LA LIBRERIA DE READLINE#####
%.o: %.c $(MINISHELL)
	@$(CC) $(CFLAGS) -o $@ -c $< $(HEADERS)
	@echo "$(GREEN) ✓ Compiled: $(notdir $<)"

line:
	@echo "$(GREEN) $(BOLD)"
	@echo " COMPILING MINSHELL...$(END) $(GREEN)"
	@echo "✦ ---------------------- ✦"

clean:
	@echo "\n$(YELLOW) 🗑  Removing objects...$(END)"
	@$(RM) $(OBJS) $(OBJS_PARSER) $(OBJS_EXEC)
	@echo "$(GREEN)\r✓ $(RED)Removed objects from $(NAME) $(END)"
	@make clean -s -C $(LIBFT_DIR)
	
fclean: clean
	@echo "\n$(YELLOW) 🗑  Removing $(NAME)...$(END)"
	@$(RM) $(NAME)
	@make fclean -s -C $(LIBFT_DIR)
	@echo "$(GREEN)\r✓ $(RED)Removed $(NAME) $(END)\n"

re: fclean all

.PHONY: all, clean, fclean, re, libft