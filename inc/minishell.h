/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: druina <druina@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/26 14:20:06 by druina            #+#    #+#             */
/*   Updated: 2023/08/17 08:59:39 by druina           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h> // exit()
#include <limits.h> // UINT_MAX
#include <readline/readline.h> // with Mac specific path in the Makefile...
#include <readline/history.h> // ...READLINE_INC & READLINE_LIB
#include <fcntl.h> // open()
# include <stdlib.h>

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 3
# endif

# define PROMPT "\e[1;37m\e[44m \xF0\x9F\x90\x9A [minishell] > \e[0m "
# define SPACE ' '
# define PIPE '|'
# define SGL_QUOTE '\''
# define DBL_QUOTE '"'

typedef struct s_list
{
	char			*content;
	struct s_list	*next;
}					t_list;

typedef enum	e_mode
{
	INTERACTIVE,
	NON_INTERACTIVE,
}				t_mode;

typedef struct	e_minishell
{
	t_mode		mode;
	char		**commands;
	int			nb_command;
}				t_minishell;

//  get_next_line_utils.c
int		ft_strlen(char *str);
int		ft_strchr(char *str, char c);
char	*ft_strjoin(char *save, char *buf);
char	*ft_strndup(char *str, int n);

// get_next_line.c
char	*clean_save(char *save);
char	*get_line(char *save);
char	*read_and_save(char *save, int fd);
char	*get_next_line(int fd);

// get_commands.c
char		**get_commands(char *line, char c);
int			ft_nb_command(char *line, char c);
int			ft_command_len(char *s, char c, int i, char quote);
char		**ft_command_split(char *s, char c, char **result, int nb_command);
char		**ft_split(char *s, char c, int nb_command);
char		check_quote(char letter, char quote);

// main.c
t_minishell	*get_minishell(t_minishell *minishell);
t_minishell	*parse(char *line);


// init.c
char		*init_prompt(int fd, t_mode mode);
t_minishell	*init_minishell(int argc);
int			open_or_exit(char *file);

// lexer.c


// utils.c
void		*calloc_or_exit(size_t size, int count);
void		ft_bzero(void *memory, size_t size);
void		*ft_calloc(size_t count, size_t size);
t_minishell	*get_minishell(t_minishell *minishell);

#endif
