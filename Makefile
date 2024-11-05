# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: gbeaudoi <gbeaudoi@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/01/08 14:30:03 by gbeaudoi          #+#    #+#              #
#    Updated: 2024/11/05 13:14:54 by gbeaudoi         ###   ########.fr        #
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
CFLAGS =	-Wall -Wextra -Werror -I $(INC)
DEP_FLAGS	=	-MMD -MP
INC		=	include/
LIBC	=	ar -rcs
RM		=	rm -rf


################################### SOURCES ###################################

PARSING_DIR 	= 	parsing/
PARSING_FILES	= 	expendeur.c parseur.c parsing_utils.c quote_handling.c node_init.c node_utils.c redir_init.c redistribute_node.c \
					free_parsing.c free_utils.c expendeur_helper.c

BUILTIN_DIR 	= 	builtin/
BUILTIN_FILES	= 	cd.c echo.c env.c pwd.c export.c export_utils.c exit.c unset.c
					
INIT_DIR 		= 	init/
INIT_FILES		= 	main.c init.c init_no_env.c init_env_nodes.c signals.c free_memory.c utils.c  \
				 	init_arg.c free_memory_loop.c utils_libft.c

EXEC_DIR 		= 	exec/
EXEC_FILES		= 	exec_children.c exec_redirections.c executing.c prep_exec.c prep_path.c prep_path_set.c \
					set_redirections.c heredoc.c heredoc_exp.c heredoc_exp_helpeur.c


######################## COMBINE DIRECTORIES AND FILES ########################

SRC_DIR		= src/

SRC_NAMES	= $(addprefix $(INIT_DIR), $(INIT_FILES)) \
			$(addprefix $(PARSING_DIR), $(PARSING_FILES)) \
			$(addprefix $(BUILTIN_DIR), $(BUILTIN_FILES)) \
			$(addprefix $(EXEC_DIR), $(EXEC_FILES)) \


OBJ_DIR		= obj/

OBJ_NAMES	= $(SRC_NAMES:.c=.o)

OBJ_FOLDERS	= $(addprefix $(OBJ_DIR), $(INIT_DIR)) \
			 $(addprefix $(OBJ_DIR), $(PARSING_DIR))	\
			 $(addprefix $(OBJ_DIR), $(BUILTIN_DIR))	\
			 $(addprefix $(OBJ_DIR), $(EXEC_DIR))	\

OBJ		= $(addprefix $(OBJ_DIR), $(OBJ_NAMES))

#################################### MANDATORY ####################################

$(OBJ_DIR)%.o: $(SRC_DIR)%.c
			@mkdir -p $(dir $@)
			@printf "$(ITAL)$(GREEN)Compiling: $(RESET)$(ITAL)$<\n"
			@$(CC) $(DEP_FLAGS) $(CFLAGS) -c $< -o $@


$(MINISHELL): $(OBJ)
	@make -C $(LIBFT)
	@cp $(LIBFT)/libft.a .
	$(CC) $(DEP_FLAGS) $(CFLAGS)  $^ -o $@ -L$(LIBFT) -lft -lreadline

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