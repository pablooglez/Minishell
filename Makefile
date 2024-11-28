# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: pablogon <pablogon@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/10/01 19:11:08 by pablogon          #+#    #+#              #
#    Updated: 2024/11/28 01:00:01 by pablogon         ###   ########.fr        #
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

HEADERS := -I include -I$(LIBFT_DIR)

################################################################################
##                              SOURCE AND OBJECTS                            ##
################################################################################

SRCS	:= src/env_vars.c \
			src/error.c \
			src/free_utils.c \
			src/main.c \
			src/signals.c \
			src/utils.c \

SRCS_EXEC = src/src_exec/built-ins/built-ins.c\
			src/src_exec/built-ins/cd.c \
			src/src_exec/built-ins/echo.c \
			src/src_exec/built-ins/env.c \
			src/src_exec/built-ins/export.c \
			src/src_exec/built-ins/pwd.c \
			src/src_exec/built-ins/unset.c \
			src/src_exec/built-ins/exit.c \
			src/src_exec/redirections.c \
			src/src_exec/execute.c \
			src/src_exec/utils.c \

SRCS_PARSER = src/src_parsing/tokenize.c src/src_parsing/handle_token.c \
				src/src_parsing/parse_input.c src/src_parsing/handle_quotes.c \
				src/src_parsing/handle_char.c src/src_parsing/handle_pipes.c \
				src/src_parsing/command_utils.c src/src_parsing/code_checks.c \
				src/src_parsing/handle_redirections.c src/src_parsing/heredoc.c \
				src/src_parsing/expand_variables.c src/src_parsing/free_utils.c \
				src/src_parsing/expand_variables_utils.c \
				src/src_parsing/expand_variables_helpers.c  \
				src/src_parsing/parse_utils.c \

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

all: head libft line $(NAME)

head:
	@echo "$(CYAN)$(BOLD)"
	@echo "\t███╗   ███╗██╗███╗   ██╗██╗███████╗██╗  ██╗███████╗██╗     ██╗     "
	@echo "\t████╗ ████║██║████╗  ██║██║██╔════╝██║  ██║██╔════╝██║     ██║     "
	@echo "\t██╔████╔██║██║██╔██╗ ██║██║███████╗███████║█████╗  ██║     ██║     "
	@echo "\t██║╚██╔╝██║██║██║╚██╗██║██║╚════██║██╔══██║██╔══╝  ██║     ██║     "
	@echo "\t██║ ╚═╝ ██║██║██║ ╚████║██║███████║██║  ██║███████╗███████╗███████╗"
	@echo "\t╚═╝     ╚═╝╚═╝╚═╝  ╚═══╝╚═╝╚══════╝╚═╝  ╚═╝╚══════╝╚══════╝╚══════╝"
	@echo "$(END)"
	@echo "$(YELLOW)$(BOLD)\t🛠️ 42MLG: $(END)$(WHITE)albelope & pablogon$(END)"
	@echo "$(PURPLE)\t📂 Project:$(END) $(CYAN)minishell$(END)"
	@echo "$(GREEN)\t🔹 Commands: $(BOLD)all clean fclean re$(END)"
	@echo "$(BLUE)\t🧰 Compiler: $(BOLD)$(CC)$(END)\n"

libft:
	@echo "$(CYAN)🔧 Building Libft...$(END)\n"
	@make -s -C $(LIBFT_DIR)

$(NAME): $(OBJS) $(OBJS_PARSER) $(OBJS_EXEC)
	@echo "$(BLUE)✦ ---------------------- ✦$(END)\n"
	@$(CC) $(CFLAGS) $(OBJS) $(OBJS_PARSER) $(OBJS_EXEC) $(LIBFT) $(HEADERS) $(LINK) -lreadline -o $(NAME)
	@echo "$(GREEN)✓ $(NAME) built successfully!$(END)\n"

%.o: %.c $(MINISHELL)
	@$(CC) $(CFLAGS) -o $@ -c $< $(HEADERS)
	@echo "$(GREEN) ✓ Compiled: $(notdir $<)$(END)" 

line:
	@echo "$(GREEN)$(BOLD)"
	@echo " 🚀 COMPILING MINISHELL...$(END) $(GREEN)"
	@echo "✦ ---------------------- ✦$(END)"

clean:
	@echo "\n$(YELLOW)🧹 Cleaning up object files...$(END)"
	@$(RM) $(OBJS) $(OBJS_PARSER) $(OBJS_EXEC)
	@echo "$(RED)✓ Removed objects from $(NAME)$(END)"
	@make clean -s -C $(LIBFT_DIR)

fclean: clean
	@echo "\n$(YELLOW)🗑️ Full cleanup: removing $(NAME) executable...$(END)"
	@$(RM) $(NAME)
	@make fclean -s -C $(LIBFT_DIR)
	@echo "$(RED)✓ Removed $(NAME) executable$(END)\n"

re: fclean all

.PHONY: all clean fclean re libft
