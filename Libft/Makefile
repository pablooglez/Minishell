# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: pablogon <pablogon@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/10/04 18:24:23 by pablogon          #+#    #+#              #
#    Updated: 2024/11/27 22:39:49 by pablogon         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

################################################################################
##                              COMPILATION INFO                              ##
################################################################################
LIB = ar rcs
RM = rm -f

CC = cc
CFLAGS = -Wall -Wextra -Werror

NAME = libft.a

################################################################################
##                              SOURCE AND OBJECTS                            ##
################################################################################

SRCS =  ft_bzero.c ft_isalpha.c ft_isdigit.c ft_memcpy.c ft_memchr.c \
ft_memmove.c ft_memset.c ft_memcmp.c ft_strlen.c ft_strlcpy.c \
ft_strlcat.c ft_strchr.c ft_strrchr.c ft_strnstr.c ft_strncmp.c \
ft_atoi.c ft_isalnum.c ft_isascii.c ft_isprint.c ft_toupper.c \
ft_tolower.c ft_calloc.c ft_strdup.c ft_substr.c ft_strjoin.c \
ft_strtrim.c ft_split.c ft_itoa.c ft_strmapi.c ft_striteri.c \
ft_putchar_fd.c ft_putstr_fd.c ft_putendl_fd.c ft_putnbr_fd.c \
ft_strcmp.c 


BSRCS = ft_lstnew_bonus.c ft_lstadd_front_bonus.c ft_lstsize_bonus.c ft_lstlast_bonus.c ft_lstadd_back_bonus.c \
ft_lstdelone_bonus.c ft_lstclear_bonus.c ft_lstiter_bonus.c ft_lstmap_bonus.c \


OFILES = $(SRCS:.c=.o)
BOFILES = $(BSRCS:.c=.o)
INCLUDE = libft.h

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

all: $(NAME)

$(NAME): $(OFILES) $(INCLUDE)
	@$(LIB) $(NAME) $(OFILES)
	@echo "$(GREEN)$(BOLD)✓ Successfully compiled Libft!$(END)"

bonus: $(NAME) $(BOFILES) $(INCLUDE)
	@$(LIB) $(NAME) $(BOFILES)
	@echo "$(GREEN)✓ Bonus files compiled!$(END)"

.c.o:
	@$(CC) $(CFLAGS) -c $< -o $@

clean:
	@$(RM) $(OFILES)
	@echo "$(RED)✓ Removed object files$(END)"

fclean: clean
	@$(RM) $(NAME)
	@echo "$(RED)✓ Removed $(NAME)$(END)"

re: fclean all

.PHONY: all clean fclean re bonus

