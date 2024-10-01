# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: pablogon <pablogon@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/10/01 19:11:08 by pablogon          #+#    #+#              #
#    Updated: 2024/10/01 19:30:14 by pablogon         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME:= minishell

CFLAGS:= -g -Wall -Werror -Wextra \
		#-g -fsanitize=address,undefined \
		#-Wunreachable-code -Ofast \

# Libft files and directories
LIBFT_DIR = ./Libft
LIBFT = $(LIBFT_DIR)/libft.a

HEADERS := -I$(LIBFT)

SRCS	:= 

OBJS	:= ${SRCS:.c=.o}

all: libft $(NAME)
libft:
	@make -C $(LIBFT_DIR)

%.o: %.c
	@$(CC) $(CFLAGS) -o $@ -c $< $(HEADERS)
$(NAME): $(OBJS)
	@$(CC) $(CFLAGS) $(OBJS) $(LIBFT) $(HEADERS) $(LINK) -o $(NAME)
	@echo "Compiling minishell..."

clean:
	@rm -rf $(OBJS)
	@rm -rf $(LIBFT_DIR)/*.o

fclean: clean
	@rm -rf $(NAME)
	@rm -rf $(LIBFT)

re: fclean all

.PHONY: all, clean, fclean, re, libft