# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: gbeaudoi <gbeaudoi@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/12/23 17:08:22 by gbeaudoi          #+#    #+#              #
#    Updated: 2024/11/05 13:14:00 by gbeaudoi         ###   ########.fr        #
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

NAME		=	libft.a
CC		=	cc
CFLAGS		=	-Wall -Wextra -Werror -I

INC		=	include/
LIBC		=	ar -rcs
RM		=	rm -rf


################################### SOURCES ###################################

LIBFT_DIR	= 	libft_ori/
LIBFT_FILES	= 	ft_atoi.c ft_bzero.c ft_calloc.c ft_isalnum.c ft_isalpha.c ft_isascii.c ft_isdigit.c ft_isprint.c ft_itoa.c \
			ft_memchr.c ft_memcmp.c ft_memcpy.c ft_memmove.c ft_memset.c ft_putchar_fd.c ft_putendl_fd.c \
			ft_putnbr_fd.c ft_putstr_fd.c ft_split.c ft_strchr.c ft_strdup.c ft_striteri.c ft_strjoin.c ft_strlcat.c ft_strlcpy.c ft_strlen.c \
			ft_strmapi.c ft_strncmp.c ft_strnstr.c ft_strrchr.c ft_strtrim.c ft_substr.c ft_tolower.c ft_toupper.c \
			ft_lstnew.c ft_lstadd_front.c ft_lstsize.c ft_lstlast.c ft_lstadd_back.c ft_lstdelone.c ft_lstclear.c ft_lstiter.c ft_lstmap.c

GNL_DIR 	= 	get_next_line/
GNL_FILES	= 	get_next_line_bonus.c get_next_line_utils_bonus.c

FT_PRINTF_DIR	=	ft_printf/
FT_PRINTF_FILES	=	ft_printf.c ft_utility.c ft_utility2.c ft_utility3.c ft_flags.c ft_print_char.c \
			ft_print_string.c ft_print_pointer.c ft_print_intdec.c ft_print_uns_dec.c ft_print_hex.c

######################## COMBINE DIRECTORIES AND FILES ########################

SRC_DIR		= src/

SRC_NAMES	= $(addprefix $(LIBFT_DIR), $(LIBFT_FILES)) \
			$(addprefix $(GNL_DIR), $(GNL_FILES)) \
			$(addprefix $(FT_PRINTF_DIR), $(FT_PRINTF_FILES))

OBJ_DIR		= obj/

OBJ_NAMES	= $(SRC_NAMES:.c=.o)

OBJ_FOLDERS	= $(addprefix $(OBJ_DIR), $(LIBFT_DIR)) \
				$(addprefix $(OBJ_DIR), $(GNL_DIR))	\
				$(addprefix $(OBJ_DIR),  $(FT_PRINTF_DIR))

OBJ		= $(addprefix $(OBJ_DIR), $(OBJ_NAMES))

#################################### RULES ####################################

$(OBJ_DIR)%.o: $(SRC_DIR)%.c
			@mkdir -p $(dir $@)
			@printf "$(ITAL)$(GREEN)Compiling: $(RESET)$(ITAL)$<\n"
			@$(CC) $(CFLAGS) $(INC) -c $< -o $@

# link .o files to the library
$(NAME):	$(OBJ)
			@$(LIBC) $(NAME) $(OBJ)
			@printf "\n$(RESET)$(BOLD)$(PINK)[LIBFT]$(RESET), $(BOLD)$(BLUE)[GET_NEXT_LINE]$(RESET), $(BOLD)$(RED)[FT_PRINTF]$(RESET) successfully compiled!$(RESET)\n"

all:		$(NAME)


clean:
			@$(RM) $(OBJ_DIR)
			@printf "$(BOLD)$(PINK)[LIBFT]: $(RESET)$(PINK)object files $(RESET)\t\t=> CLEANED!\n\n"

fclean: 	clean
			@$(RM) $(NAME)
			@find . -name ".DS_Store" -delete
			@printf "$(BOLD)$(PURPLE)[LIBFT]: $(RESET)$(PURPLE)exec. files $(RESET)\t=> CLEANED! \n\n"

re:		fclean all
			@printf "\n\n $(BOLD)$(YELLOW)Cleaning and rebuilding done! $(RESET)\n"


.PHONY:		all clean fclean re norm