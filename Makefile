# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: pablogon <pablogon@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/10/01 19:11:08 by pablogon          #+#    #+#              #
#    Updated: 2024/10/02 21:15:23 by pablogon         ###   ########.fr        #
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

HEADERS := -I include -I$(LIBFT)

################################################################################
##                              SOURCE AND OBJECTS                            ##
################################################################################

SRCS	:= 

SRCS_PARSER =

SRCS_EXEC = 

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
	@echo "\t███    ███ ██ ███    ██ ██ ███████ ██   ██ ███████ ██             "
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
	@make -C $(LIBFT_DIR)

$(NAME): line $(OBJS) $(OBJS_PARSER) $(OBJS_EXEC)
	@echo "✦ ---------------------- ✦$(END)"
	@$(CC) $(CFLAGS) $(OBJS) $(OBJS_PARSER) $(OBJS_EXEC) $(LIBFT) $(HEADERS) $(LINK) -o $(NAME)

%.o: %.c $(MINISHELL)
	@$(CC) $(CFLAGS) -o $@ -c $< $(HEADERS)
	@echo "$(GREEN) ✓ Compiled: $(notdir $<)"

line:
	@echo "$(GREEN) $(BOLD)"
	@echo " Compiling Minishell...$(END) $(GREEN)"
	@echo "✦ ---------------------- ✦"

clean:
	@printf "\n$(YELLOW) 🗑   Removing objects...$(END)"
	@$(RM) $(OBJS) $(OBJS_PARSER) $(OBJS_EXEC)
	@echo "$(GREEN)\r  ✓  $(RED)Removed  objects from $(NAME) $(END)"
	@make clean -s -C $(LIBFT)
	
fclean: clean
	@printf "\n$(YELLOW) 🗑   Removing... $(NAME) $(END)"
	@$(RM) $(NAME)
	@make fclean -s -C $(LIBFT)
	@echo "$(GREEN)\r"  ✓  $(RED) Removed $(NAME) $(END)\n"

re: fclean all

.PHONY: all, clean, fclean, re, libft