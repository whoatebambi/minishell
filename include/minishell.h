/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcouserg <fcouserg@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/21 18:24:48 by gbeaudoi          #+#    #+#             */
/*   Updated: 2024/05/24 17:53:03 by fcouserg         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../libft/include/ft_printf.h"
# include "../libft/include/libft.h"
# include "../libft/include/get_next_line_bonus.h"

# include <dirent.h>
# include <errno.h>
# include <fcntl.h>
# include <limits.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <unistd.h>
#include <string.h>

// MACRO pour definir les TOKEN
# define PIPE 1001
# define LOWER 1002
# define D_LOWER 1003
# define GREATER 1004
# define D_GREATER 1005

typedef enum e_mode
{
	INTERACTIVE,
	NON_INTERACTIVE,
}			t_mode;


// Les structs que je pensais utiliser

//Utile pour toi pour quand tu codera le builtin export ou env
typedef struct s_env
{
	char		*key;
	char		*var;
}				t_env;

// Permet de savoir si la commande est lexicalement correct
typedef struct s_parseur
{
	int		double_quote;
	int		db_quote_count;
	int		single_quote;
	int		sl_quote_count;
}				t_parseur;

// Permet degarder les changement d environement qui change
typedef struct s_env_moving
{
	int			shell_lvl;
	char		*pwd;
	char		*old_pwd;
}				t_env_moving;

// permet d avoir toutes les info d une commande
typedef struct s_command
{
	char		*commands;
	int			builtin;
	int			path;
	char		**option;
	char		**words;
	char 		*redir;
}				t_command;

// permet de savoir les commande qui entoure une pipe 
typedef struct s_pipe
{
	int			index;
	t_command	left;
	t_command	right;
	struct s_pipe *next;
	struct s_pipe *previous;
}				t_pipe;

// tentative structure qui aurait toutes les autres structs afin de passer en argument dans les fonctions.
typedef struct s_shell
{
	t_mode		mode; // added inte/ non-inte mode
	char		**environment;
	t_pipe		*pipes;
	t_command	**commands;
	t_env       *env_var;
	t_parseur	*parseur;
}				t_shell;

// Suggested minishell struct, but you can keep yours if its more logical ofc
// typedef struct s_minishell
// {
// 	t_mode		mode;
// 	int			nb_cmds;
// 	char		**instructions;
// 	t_list		*env;
// 	char		**env_paths;
// 	t_cmd_table	*cmd_table;
// 	pid_t		*child_pids;
// 	int			exit_code;
// }			t_minishell;


// main.c


// executing.c
void		execute(t_shell *minishell, char *line);
void    	execute_builtin(t_command *command);
void		pwd(void);
void    	echo(char *commands);
t_command    **temp_parse_commands(char *line);


// init_parameters.c
void	check_argc(int argc);
int		init_fd(int argc, char **argv, int fd);

// utils.c
t_shell	*get_minishell(t_shell *minishell);


#endif
