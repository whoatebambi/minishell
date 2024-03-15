# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: fcouserg <fcouserg@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/11/14 18:59:07 by fcouserg          #+#    #+#              #
#    Updated: 2023/11/23 17:46:38 by fcouserg         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #


NAME				= minishell

# Directories
INC					= inc/
SRC_DIR				= src/
OBJ_DIR				= obj/

# Compiler and CFlags
CC					= cc
CFLAGS				= -Wall -Werror -Wextra -fsanitize=address
RM					= rm -f

READLINE_INC		= /opt/homebrew/opt/readline/include
READLINE_LIB		= /opt/homebrew/opt/readline/lib

SRC					= main.c utils.c get_next_line_utils.c get_next_line.c get_commands.c lexer.c init.c
OBJ					= $(SRC:%.c=$(OBJ_DIR)%.o)

all:				$(NAME)	

$(NAME):			$(OBJ)
					@$(CC) $(CFLAGS) -I$(INC) -I$(READLINE_INC) $(OBJ) -L$(READLINE_LIB) -lreadline -o $(NAME)
					@echo "\033[0;92m* ./minishell was created *\033[0m"
					

$(OBJ_DIR)%.o:		$(SRC_DIR)%.c 
					@mkdir -p $(@D)
					@$(CC) $(CFLAGS) -I$(INC) -c $< -o $@

clean:
					@$(RM) -r $(OBJ_DIR)
					@echo "\033[0;91m* object files were deleted *\033[0m"

fclean:				clean
					@$(RM) $(NAME)
					@echo "\033[0;91m* all files were deleted *\033[0m"

re:					fclean all

.PHONY:				start all clean fclean re