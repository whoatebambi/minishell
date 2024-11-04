/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbeaudoi <gbeaudoi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/21 18:24:48 by gbeaudoi          #+#    #+#             */
/*   Updated: 2024/11/04 15:17:15 by gbeaudoi         ###   ########.fr       */
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
# include <stdbool.h>
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

/* see
 * https://www.cyberciti.biz/faq/linux-bash-exit-status-set-exit-statusin-bash/
 */
# define EXIT_NOT_FOUND 127
# define EXIT_SIGINT 130
# define EXIT_SIGQUIT 131
# define EXIT_ERROR 255
# define SYNTAX_ERROR 258

typedef enum e_status
{
	INIT,
	RESET,
	CHILD_PROCESS,
}					t_status;

extern int			g_sig;

typedef enum e_mode
{
	INTERACTIVE,
	NON_INTERACTIVE,
}					t_mode;

typedef enum e_token
{
	NOT_TOKEN,
	PIPE,
	GREATER,
	D_GREATER,
	LOWER,
	D_LOWER,
}					t_token;

typedef enum e_builtin
{
	NOT,
	ECHO,
	CD,
	PWD,
	EXPORT,
	UNSET,
	ENV,
	EXIT,
}					t_builtin;

typedef struct s_lex
{
	char			*word;
	bool			istoken;
	bool			isred;
	bool			quotelim;
	int				i;
	t_token			token;
	struct s_lex	*next;
	struct s_lex	*prev;
}					t_lex;

typedef struct s_env
{
	char			*key;
	char			*value;
	int				index;
	bool			isunset;
	bool			oldpwd;
	// struct s_env	*prev;
	struct s_env	*next;
}					t_env;

typedef struct s_path
{
	char			*pwd;
	char			*oldpwd;
}					t_path;

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
	int				output;
	int				in;
	int				savedpipe;
	bool			prevpipe;
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
	char			**tab;
	char			*path;
	bool			pathnoaccess;
	t_builtin		builtin;
	char			*group_command;
	t_node			*nodes;
	t_redir			*redirs_in;
	t_redir			*redirs_out;
	int				is_infile_tmp;
	char			*infile_tmp;
}					t_cmd_table;

typedef struct s_shell
{
	t_mode			mode;
	char			**envp;
	char			**tabpath;
	t_path			*path;
	char			*execve_path;
	t_env			*env;
	t_lex			*lex;
	t_cmd_table		**cmd_table;
	pid_t			*child_pids;
	int				excode;
	int				tmpexcode;
	char			*inp;
	char			*newinp;
	char			*finalinp;
	int				inflagerr;
	int				outflagerr;
	char			*line;
	char			*clean_line;
	int				count_pipes;
}					t_shell;

// main.c
char				*get_line(t_mode mode, int fd);

// exec_system.c
void				exec_system(char **tab, t_shell *minishell);
void				prep_exec(t_shell *minishell, t_fds *fd);
char				*find_relative_path(char *arg, char **envp);

// executing.c
void				start_exec(t_shell *minishell);
int					execute_builtin(t_cmd_table *cmd_table, t_shell *minishell,
						t_fds *fd, int i);
void				ft_exec(t_shell *minishell, t_cmd_table *cmd, int i,
						t_fds *fd);
void				prep_exec(t_shell *minishell, t_fds *fd);
char				**get_execpath(t_shell *shell);
void				check_builtins(t_cmd_table *cmd);

void				ft_perror(t_shell *shell, char *word, char *msg,
						char *third);

// exec_redirections.c
void				handle_redirs(t_shell *minishell, t_fds *fd, int i);
void				set_redirs(t_fds *fd);
void				ft_init_fds(t_fds *fd);
void				close_fds(t_fds *fd);
void				close_fds_parent(t_fds *fd);
void				ft_pipes(t_shell *minishell, t_fds *fd, int i);

/////////////////// BUILTIN
// cd.c
void				builtin_cd(char **tab, t_env *env, t_shell *minishell);
void				chdir_getenv(char **tab, t_shell *minishell);
int					cd_multiple_args(char **tab, t_shell *minishell);
void				replace_env_var(char *pwd, char *key, t_env *env);
// echo.c
void				builtin_echo(char **tab, t_shell *minishell, int i);
void				echo_excode(t_shell *minishell, int i, int *ext);
int					check_newline(char **tab, int *flag);
// env.c
void				builtin_env(t_env *env, t_shell *minishell);
// exit.c
void				builtin_exit(char **tab, t_shell *minishell, t_fds *fd);
int					exit_args(char **tab, int *flag);
// export.c
void				builtin_export(t_env *env, char **tab, t_shell *minishell);
int					add_env_list(char *arg, t_env *env, t_shell *minishell);
int					export_check_syntax(int *i, char *arg, t_shell *minishell);
t_env				*get_env_lst(char *name, t_env *env);
void				print_export(t_env *env);
// export_utils.c
void				set_env_value(char *name, char *value, t_env *env);
void				create_env_list(char *name, char *value,
						t_shell *minishell);
// pwd.c
void				builtin_pwd(int fd_out, t_shell *minishell);
// unset.c
void				builtin_unset(char **tab, t_shell *minishell);

// signals.c
void				ft_handle_sig(int s);
void				ft_sig_heredoc(int s);
void				ft_sig_heredoc_setting(void);
void				ft_signals(void);
int					ft_catchsignals(t_shell *minishell);

// initializing.c
int					init_argc(int argc, char **argv, int fd);
void				init_minishell(t_shell *minishell, char **envp, int argc);
void				fill_env(t_shell *minishell, char **envp);
// t_env				*add_env_var(char *envp);
// char				**add_oldpwd(char **envp);
void				fill_envp(t_shell *minishell);
void				fill_path(t_shell *minishell);
int					env_size(t_shell *shell);
char				*getpath(t_shell *minishell, char *key);

// init_no_env.c
void				set_pwd(t_shell *shell);
void				set_shlvl(t_shell *shell);
void				set_lastcmd(t_shell *shell);
void				set_oldpwd(t_shell *shell);
void				ft_no_env(t_shell *minishell);

// init_env_nodes.c
void				init_env_nodes(t_shell *minishell, t_env *new_node,
						char **envp, int i);
void				set_shlvl_inception(t_shell *minishell, t_env *node);

// utils.c
t_shell				*get_minishell(t_shell *minishell);
void				safe_write(int fd, const char *str, ...);
int					safe_strcmp(char *s1, char *s2);
void				swap_env(t_list *a, t_list *b);
void				ft_exit_msg(t_shell *minishell, char *errmsg);
char				*safe_join_envp(char *key, char *symb, char *value);

// free_memory.c
void				free_env(t_env *env);
void				free_path(t_path *path);
void				free_cmd_table(t_shell *minishell);
void				free_minishell(t_shell *minishell);
void				reset_loop(t_shell *minishell);
void				exitmsg(t_shell *minishell, char *errmsg);
void				ft_free_child(pid_t *i);
void				ft_free_double_char(char **tab);
void				safe_free(char *string);
void				ft_free_redir(t_redir *redir_def);
void				ft_free_node(t_node *node_def);
void				ft_check_strdup(char *str, int i, char **dest, int flag);

// Parseur expendeur llexeur
int				ft_parseur(t_shell *minishell);
void				ft_dollar_option(char *copy, t_shell *minishell, int i,
						int flag_dbl);
void				ft_expand_dollar(t_shell *minishell, int delim,
						int flag_sgl, int flag_dbl);
void				ft_rev_neg_line(t_shell *minishell);
char				*ft_strndup(char *str, int n);
void				ft_neg_inside_quote(t_shell *minishell);
int					ft_parseur_quote(t_shell *minishell);
char				*ft_strjoin_no_free(char *s1, char *s2);
int					ft_strcmp(const char *s1, const char *s2);
void				ft_init_node_list(t_node **nodes,
						t_cmd_table *command_table, t_shell *minishell);
int					ft_rev_neg_words(char *line);
void				ft_tokenize_list(t_node *nodes, t_shell *minishell);
void				ft_check_syntax(t_node *nodes, t_shell *minishell);
void				ft_redistribute_node(t_cmd_table **command_table,
						t_node *nodes, t_shell *minishell);
void				ft_init_redir_list_in(t_redir **redir, t_node *nodes,
						t_shell *minishell);
void				ft_init_redir_list_out(t_redir **redir, t_node *nodes,
						t_shell *minishell);
// void				tokenizer(t_shell *minishell);
// void				link_lex_nodes(t_shell *shell, t_lex *new, int *i, int *j);

// HEREDOC
int					ft_here_doc_exp(t_shell *minishell, t_redir *copy_in,
						t_fds *fd);
int					ft_here_doc(t_shell *minishell, t_redir *copy_in,
						t_fds *fd);
char				*ft_find_tmp_heredoc(t_shell *minishell, char *heredoc);
void				ft_ctrlc(t_shell *minishell, t_fds *fd, char *heredoc);

// NODE INIT
t_node				*ft_new_node(char *word, int flag, t_shell *minishell);
t_redir				*ft_last_stack_redir(t_redir *a);
void				ft_stack_add_to_back(t_node **a, t_node *new_node);

#endif
