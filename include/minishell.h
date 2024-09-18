/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcouserg <fcouserg@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/21 18:24:48 by gbeaudoi          #+#    #+#             */
/*   Updated: 2024/09/18 18:46:03 by fcouserg         ###   ########.fr       */
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
# include <stdarg.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/stat.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <unistd.h>

# define ERRARG "No args allowed"
# define MERROR "Malloc error"
# define SYNTERR "syntax error near unexpected token"
# define NOSUCH "No such file or directory"
# define CMDFAIL "command not found"
# define NOTV "not a valid identifier"
# define E "export"

extern int			g_sig;

typedef enum e_mode
{
	INTERACTIVE,
	NON_INTERACTIVE,
}					t_mode;

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

typedef struct s_fds
{
	int				pipes[2];
	int				redir[2];
	int				input;
	int				in;
	int				output;
}					t_fds;

typedef struct s_redir
{
	t_redir_type	type;
	char			*redir_name;
	int				quote;
	struct s_redir	*next;
	struct s_redir	*previous;
}					t_redir;

// permet d avoir toutes les info d une commande
typedef struct s_node
{
	char			*string;
	int token; // 0 no, 1 token
	int redir; // char	*redir;
	int				quote;
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
	int				is_infile_tmp;
	char			*infile_tmp;
}					t_cmd_table;

// tentative structure qui aurait toutes les autres structs afin de passer en argument dans les fonctions.
typedef struct s_shell
{
	t_mode			mode;
	char			**env;
	t_cmd_table		**cmd_table;
	t_list			*env_lst;
	pid_t			*child_pids;
	int				tmp_exit_code;
	int				exit_code;
	char			*line;
	char			*clean_line;
	int				count_pipes;
}					t_shell;

// main.c
char				*get_line(t_mode mode, int fd);

// exec_system.c
void				exec_system(char **cmd_args, t_list *env_lst);
char				**build_execve_envp(t_list *env_lst);
char				**build_execve_path(t_list *env_lst);
char				*find_relative_path(char *arg, char **execve_path_table);

// executing.c
void				execute(t_shell *minishell);
int					execute_builtin(t_cmd_table *cmd_table, t_list *env_lst,
						t_fds *fd);
void				exec_in_child(t_shell *minishell, int i, t_fds *fd);

// exec_redirections.c
void				exec_redirs(t_shell *minishell, t_fds *fd, int i);
void				set_redirs(t_fds *fd);
void				ft_init_fds(t_fds *fd);
void				close_fds(t_fds *fd);
void				close_fds_parent(t_fds *fd);

// builtin.c
int					pwd(int fd_out);
int					cd(char **cmd_args, t_list *env_lst);
void				replace_env_var(char *pwd, char *key, t_list *env_lst);
int					echo(char **cmd_args, int fd_out);
int					check_newline(char *cmd_arg);
int					env(t_list *env_lst, int fd_out);
int					export(t_list *env_lst, t_cmd_table *cmd_table, int fd_out);
t_list				*dup_env_lst(t_list *env_lst);
int					print_export(t_list *env_lst, int fd_out);
void				sort_env_lst(t_list **list);
int					is_builtin(char *cmd_arg);

// initializing.c
int					init_argc(int argc, char **argv, int fd);
t_shell				*init_minishell(char **envp, int argc);
int					init_fd(int argc, char **argv, int fd);
t_list				*init_env_lst(char **envp);
t_env				*add_env_var(char *envp);
char				**add_oldpwd(char **envp);

// utils.c
t_shell				*get_minishell(t_shell *minishell);
int					safe_write(int fd, const char *str, ...);
int					safe_strcmp(char *s1, char *s2);
void				swap_env(t_list *a, t_list *b);
void				ft_exit_msg(t_shell *minishell, char *errmsg);

// signals.c
// void				set_signals(t_mode mode);
// void				sig_handler(int signum);
void				ft_handle_sig(int s);
void				ft_sig_heredoc(int s);
void				ft_sig_heredoc_setting(void);
void				ft_signals(void);
int					ft_catchsignals(t_shell *minishell);

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
int					ft_rev_neg_words(char *line);
void				ft_tokenize_list(t_node *nodes);
void				ft_check_syntax(t_node *nodes);
void				ft_redistribute_node(t_cmd_table **command_table,
						t_node *nodes);
void				ft_init_redir_list(t_redir **redir, t_node *nodes,
						char *token1, char *token2);

// HEREDOC
int					ft_here_doc_exp(t_shell *minishell, t_redir *copy_in,
						t_fds *fd);
int					ft_here_doc(t_shell *minishell, t_redir *copy_in,
						t_fds *fd);
char				*ft_find_tmp_heredoc(t_shell *minishell, char *heredoc,
						t_redir *copy_in);
void				ft_ctrlc(t_shell *minishell, t_fds *fd, char *heredoc);

// NODE INIT
t_node				*ft_new_node(char *word, int flag);
void				ft_stack_add_to_back(t_node **a, t_node *new_node);

// free
void				ft_free_cmd_table_loop(t_cmd_table **cmd_table,
						int count_pipes);
void				ft_free_cmd_table_final(t_cmd_table **cmd_table,
						int count_pipes);
void				ft_free_child(pid_t *i);
void				ft_free_double_char(char **tab);
void				ft_free_line(char *string);
void				ft_free_redir(t_redir *redir_def);
void				ft_free_node(t_node *node_def);
void				ft_check_strdup(char *str, int i, char **dest, int flag);

#endif
