/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcouserg <fcouserg@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/21 18:24:48 by gbeaudoi          #+#    #+#             */
/*   Updated: 2024/06/04 17:03:26 by fcouserg         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../libft/include/ft_printf.h"
# include "../libft/include/get_next_line_bonus.h"
# include "../libft/include/libft.h"
# include <dirent.h>
# include <errno.h>
# include <fcntl.h>
# include <limits.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <unistd.h>

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
}					t_mode;

// Les structs que je pensais utiliser

// Utile pour toi pour quand tu codera le builtin export ou env
typedef struct s_env
{
	char			*key;
	char			*var;
}					t_env;

// Permet degarder les changement d environement qui change
typedef struct s_env_moving
{
	int				shell_lvl;
	char			*pwd;
	char			*old_pwd;
}					t_env_moving;

typedef enum e_redir_type
{
	REDIR_IN,
	REDIR_OUT,
	DELIMITER,
	APPEND,
}				t_redir_type;

typedef struct s_redir
{
	t_redir_type	type;
	char			*redir;
	struct s_node 	*next;
	struct s_node 	*previous;
}					t_redir;

// permet d avoir toutes les info d une commande
typedef struct s_node
{
	char			*string;
	int 			token;
	char			*redir;
	struct s_node 	*next;
	struct s_node 	*previous;
}					t_node;

// permet d avoir toutes les info d une commande
typedef struct s_cmd_table
{
	char	*group_command;
	t_node	*nodes;
	char	**cmd_args;
	t_redir *redirs;
	int		fd_in;
	int		fd_out;
}					t_cmd_table;

// permet de savoir les commande qui entoure une pipe
typedef struct s_pipe
{
	int				index;
	t_cmd_table		*left;
	t_cmd_table		*right;
	struct s_pipe	*next;
	struct s_pipe	*previous;
}					t_pipe;

// tentative structure qui aurait toutes les autres structs afin de passer en argument dans les fonctions.
typedef struct s_shell
{
	t_mode			mode;
	char			**env;
	t_pipe			*pipes; // c'est utile ?
	t_cmd_table		**cmd_table;
	t_list			*env_lst;
	pid_t			*child_pids;
	int				exit_code;
	char			*line;
	char			*clean_line;
}					t_shell;

// main.c
char				*get_line(t_mode mode, int fd);

// executing.c
void				execute(t_shell *minishell, char *line);
void				execute_builtin(t_cmd_table *cmd_table, t_list *env_lst);
void				pwd(void);
void				echo(char **cmd_args);
void				env(t_list *env_lst);
t_cmd_table			**temp_parse_commands(char *line);

// initializing.c
int					init_argc(int argc, char **argv, int fd);
t_shell				*init_minishell(char **envp, int argc);
int					init_fd(int argc, char **argv, int fd);
t_list				*init_env_lst(char **envp);
t_env				*add_env_var(char *envp);

// utils.c
t_shell				*get_minishell(t_shell *minishell);

// signals.c
void				set_signals(t_mode mode);
void				sig_handler(int signum);

// free_memory.c
void				free_env(void *content);
void				free_env_lst(t_list *env_lst);
void				free_cmd_table(t_cmd_table **cmd_table);
void				free_minishell(t_shell *minishell);

// Parseur expendeur llexeur
void				ft_parseur(t_shell *minishell);
void				ft_expand_dollar(t_shell *minishell, int i);
void				ft_rev_neg_line(t_shell *minishell);
char				*ft_strndup(char *str, int n);
void				ft_neg_inside_quote(t_shell *minishell, int i);
int					ft_parseur_quote(t_shell *minishell);
char				*ft_strjoin_no_free(char *s1, char *s2);
int					ft_strcmp(const char *s1, const char *s2);

#endif
