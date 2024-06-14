/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcouserg <fcouserg@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/21 18:24:48 by gbeaudoi          #+#    #+#             */
/*   Updated: 2024/06/14 16:53:44 by fcouserg         ###   ########.fr       */
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
}					t_redir_type;

typedef struct s_redir
{
	t_redir_type	type;
	char			*redir_name;
	struct s_redir	*next;
	struct s_redir	*previous;
}					t_redir;

// permet d avoir toutes les info d une commande
typedef struct s_node
{
	char			*string;
	int token; // 0 no, 1 token
	int redir; // char	*redir;
	struct s_node	*next;
	struct s_node	*previous;
}					t_node;

// permet d avoir toutes les info d une commande
typedef struct s_cmd_table
{
	char			*group_command;
	t_node			*nodes;
	char			**cmd_args;
	t_redir			*redirs_in;
	t_redir			*redirs_out;
	int				fd_in;
	int				fd_out;	
	int				fd_pipe[2];
}					t_cmd_table;

// tentative structure qui aurait toutes les autres structs afin de passer en argument dans les fonctions.
typedef struct s_shell
{
	t_mode			mode;
	char			**env;
	t_cmd_table		**cmd_table;
	t_list			*env_lst;
	pid_t			*child_pids;
	int				exit_code;
	char			*line;
	char			*clean_line;

	int				count_pipes;
}					t_shell;

// main.c
char				*get_line(t_mode mode, int fd);

// build_execve.c
char				**build_execve_envp(t_list *env_lst);
char	**build_execve_path(t_list *env_lst);

// executing.c
void				execute(t_shell *minishell, char *line);
void				execute_builtin(t_cmd_table *cmd_table, t_list *env_lst);
void				exec_in_child(t_shell *minishell, int i);
void				exec_system(t_shell *minishell, int i);

// exec_redirections.c
void				exec_redirs(t_cmd_table *cmd, t_redir *redir_in, t_redir *redir_out);
void				exec_redirs_out(t_cmd_table *cmd, t_redir *redir_out);
void				exec_redirs_in(t_cmd_table *cmd, t_redir *redir_out);

// builtin.c
void				pwd(int fd_out);
void				echo(char **cmd_args, int fd_out);
void				env(t_list *env_lst, int fd_out);
void				export(t_list *env_lst, t_cmd_table *cmd_table, int fd_out);
t_list				*dup_env_lst(t_list *env_lst);
void				print_export(t_list *env_lst, int fd_out);
int					is_builtin(char *cmd_arg);

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
void				free_minishell_loop(t_shell *minishell);

// Parseur expendeur llexeur
void				ft_parseur(t_shell *minishell);
void				ft_expand_dollar(t_shell *minishell, int i);
void				ft_rev_neg_line(t_shell *minishell);
char				*ft_strndup(char *str, int n);
void				ft_neg_inside_quote(t_shell *minishell, int i);
int					ft_parseur_quote(t_shell *minishell);
char				*ft_strjoin_no_free(char *s1, char *s2);
int					ft_strcmp(const char *s1, const char *s2);
void				ft_init_node_list(t_node **nodes,
						t_cmd_table *command_table);
void				ft_rev_neg_words(char *line);
void				ft_tokenize_list(t_node *nodes);
void				ft_check_syntax(t_node *nodes);
void				ft_redistribute_node(t_cmd_table **command_table,
						t_node *nodes);
void				ft_init_redir_list(t_redir **redir, t_node *nodes,
						char *token1, char *token2);

// free
void				ft_free_cmd_table_loop(t_cmd_table **cmd_table,
						int count_pipes);
void				ft_free_cmd_table_final(t_cmd_table **cmd_table,
						int count_pipes);
void				ft_free_child(pid_t *i);
void				ft_free_int(int i);
void				ft_free_double_char(char **tab);
void				ft_free_line(char *string);
void				ft_free_redir(t_redir *redir_def);
void				ft_free_node(t_node *node_def);

#endif
