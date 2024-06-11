# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: gbeaudoi <gbeaudoi@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/01/08 14:30:03 by gbeaudoi          #+#    #+#              #
#    Updated: 2024/06/10 17:26:14 by gbeaudoi         ###   ########.fr        #
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

PARSING_DIR 	= 	parsing/
PARSING_FILES	= 	expendeur.c parseur.c parsing_utils.c quote_handling.c list_utils.c redir_list.c redistribute_node.c \
					 free_parsing.c free_utils.c

OTHER_DIR 	= 	other/
OTHER_FILES	= 	 main.c initializing.c executing.c signals.c free_memory.c utils.c


######################## COMBINE DIRECTORIES AND FILES ########################

SRC_DIR		= src/

SRC_NAMES	= $(addprefix $(OTHER_DIR), $(OTHER_FILES)) \
			$(addprefix $(PARSING_DIR), $(PARSING_FILES)) \

OBJ_DIR		= obj/

OBJ_NAMES	= $(SRC_NAMES:.c=.o)

OBJ_FOLDERS	= $(addprefix $(OBJ_DIR), $(OTHER_DIR)) \
				$(addprefix $(OBJ_DIR), $(PARISNG_DIR))	\

OBJ		= $(addprefix $(OBJ_DIR), $(OBJ_NAMES))

#################################### MANDATORY ####################################

$(OBJ_DIR)%.o: $(SRC_DIR)%.c
			@mkdir -p $(dir $@)
			@printf "$(ITAL)$(GREEN)Compiling: $(RESET)$(ITAL)$<\n"
			@$(CC) $(CFLAGS) $(INC) -c $< -o $@


$(MINISHELL): $(OBJ)
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