# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: gbeaudoi <gbeaudoi@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/01/08 14:30:03 by gbeaudoi          #+#    #+#              #
#    Updated: 2024/02/27 20:13:40 by gbeaudoi         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

################################### COLORS #####################################

RESET		:=	\e[0m
BOLD		:=	\e[1m
DIM		:=	\e[2m
ITAL		:=	\e[3m
UNDERLINE	:=	\e[4m

BLACK		:=	\e[30m
GRAY		:=	\e[90m
RED		:=	\e[31m
GREEN		:=	\e[32m
YELLOW		:=	\e[33m
ORANGE		:=	\e[38;5;208m
BLUE		:=	\e[34m
PURPLE		:=	\e[35m
PINK		:=	\033[38;2;255;182;193m
CYAN		:=	\e[36m

BRIGHT_BLACK	:=	\e[90m
BRIGHT_GREEN	:=	\e[92m
BRIGHT_YELLOW	:=	\e[93m
BRIGHT_BLUE	:=	\e[94m
BRIGHT_PURPLE	:=	\e[95m
BRIGHT_CYAN	:=	\e[96m


################################### BASICS ###################################

MINISHELL		=	minishell

LIBFT = libft/

CC		=	cc
# CFLAGS		=	-Wall -Wextra -Werror -I $(INC)
INC		=	include/
CFLAGS		=	-I $(INC)
LIBC		=	ar -rcs
RM		=	rm -rf


################################### SOURCES ###################################

MINISHELL_FILE	= main.c init_parameters.c executing.c # utils.c

######################## COMBINE DIRECTORIES AND FILES ########################

SRC_DIR		= src/

SRC_MINISHELL 		= 	$(addprefix $(SRC_DIR), $(MINISHELL_FILE))

OBJ_DIR		=	obj/

OBJ_MINISHELL 		= 	$(addprefix $(OBJ_DIR), $(MINISHELL_FILE:.c=.o))

#################################### MANDATORY ####################################

$(OBJ_DIR)%.o: $(SRC_DIR)%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

$(MINISHELL): $(OBJ_MINISHELL)
	@make -C $(LIBFT)
	@cp $(LIBFT)/libft.a .
	$(CC) $(CFLAGS) $^ -o $@ -L$(LIBFT) -lft -lreadline
# $(CC) $(CFLAGS) $^ -o $@ -L$(LIBFT) -lft

all:		$(MINISHELL) 

.DEFAULT_GOAL := all

#################################### RULES ####################################

clean:
		@$(RM) $(OBJ_DIR)
		@make clean -C $(LIBFT)
		@printf "$(BOLD)$(PINK)[MINITALK]: $(RESET)$(PINK)object files $(RESET)\t\t=> CLEANED! \n\n"

fclean: 	clean
			@$(RM) $(MINISHELL)
			@$(RM) $(LIBFT)/libft.a
			@$(RM) libft.a
			@printf "$(BOLD)$(PURPLE)[LIBFT]: $(RESET)$(PURPLE)exec. files $(RESET)\t=> CLEANED! \n\n"
			@printf "$(BOLD)$(BRIGHT_PURPLE)[MINITALK]: $(RESET)$(BRIGHT_PURPLE)exec. files $(RESET)\t=> CLEANED! \n\n"

re:		fclean all
			@printf "\n\n $(BOLD)$(YELLOW)Cleaning done! $(RESET)\n"
			
.PHONY:		all clean fclean re