/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbeaudoi <gbeaudoi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/21 18:24:48 by gbeaudoi          #+#    #+#             */
/*   Updated: 2024/11/05 12:04:35 by gbeaudoi         ###   ########.fr       */
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

# define EXIT_NOT_FOUND 127
# define EXIT_SIGINT 130
# define EXIT_SIGQUIT 131
# define EXIT_ERROR 255
# define SYNTAX_ERROR 258

extern int			g_sig;

typedef enum e_mode
{
	INTERACTIVE,
	NON_INTERACTIVE,
}					t_mode;

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

typedef struct s_env
{
	char			*key;
	char			*value;
	int				index;
	bool			isunset;
	bool			oldpwd;
	struct s_env	*next;
}					t_env;

typedef struct s_path
{
	char			*pwd;
	char			*oldpwd;
}					t_path;

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
	int				in;
	int				input;
	int				output;
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

typedef struct s_node
{
	char			*string;
	int				token;
	int				redir;
	int				quote;
	struct s_node	*next;
	struct s_node	*previous;
}					t_node;

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

/////////////////// EXEC
// exec_children.c
void				exec_child_builtin(t_shell *mns, t_cmd_table *cmd,
						t_fds *fd, int i);
void				handle_fd_error(t_shell *minishell, t_fds *fd);
void				handle_path_error(t_shell *minishell, t_cmd_table *cmd,
						t_fds *fd);
void				exec_child(t_shell *minishell, t_cmd_table *cmd, t_fds *fd);
// exec_redirections.c
void				init_redirections(t_shell *minishell, t_fds *fd, int i);
void				exec_redirs_out(t_shell *minishell, t_redir *copy_out,
						t_fds *fd);
void				exec_redirs_in(t_shell *minishell, t_redir *copy_in,
						t_fds *fd);
void				init_struct_fd(t_fds *fd);
void				close_fds(t_fds *fd);
// executing.c
void				launch_exec(t_shell *minishell);
void				wait_for_children(t_shell *minishell);
void				fds_parent_close(t_fds *fd);
void				execute(t_shell *minishell, t_cmd_table *cmd, int i,
						t_fds *fd);
// init_exec.c
void				init_exec(t_shell *minishell, t_fds *fd);
void				check_builtins(t_cmd_table *cmd);
void				set_pipes(t_shell *minishell, t_fds *fd, int i);
// prep_path.c
void				set_path_true(t_shell *minishell, int flag);
int					path_true(t_shell *minishell, int *i);
void				set_cmdtable_path(t_shell *minishell, t_cmd_table *c,
						char *tmp, int *flag);
void				set_cmdtable_path_still(t_shell *minishell, t_cmd_table *c);
// set_redirections.c
void				set_redirs(t_fds *fd);
/////////////////// HEREDOC
// heredoc.c
void				ft_ctrlc(t_shell *minishell, t_fds *fd, char *heredoc);
char				*ft_find_tmp_heredoc(t_shell *minishell, char *heredoc);
void				prep_heredoc(t_shell *minishell, t_fds *fd, char **heredoc,
						int *tmp);
int					exec_here_doc(t_shell *minishell, t_redir *copy_in,
						t_fds *fd);
// heredoc_exp.c
char				*ft_dollar_hd(t_shell *minishell, char *line, int *i,
						t_env *node);
char				*ft_expand_heredoc(t_shell *mn, char *line, int i);
void				prep_heredoc_exp(t_shell *minishell, t_fds *fd,
						char **heredoc, int *tmp);
int					exec_here_doc_exp(t_shell *minishell, t_redir *copy_in,
						t_fds *fd);
// heredoc_exp_helpeur.c
char				*ft_quote_neg(char *str);
char				*ft_joinfree(char *s1, char *s2);
char				*ft_strdupfree(char *s);
char				*num_true(char *line, int i);
char				*all_num_false(t_shell *minishell, char *line, int i);
///////////////////

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
///////////////////

/////////////////// INIT
// free_memory.c
void				free_specific_env(t_env *new, char *str,
						t_shell *minishell);
void				free_minishell(t_shell *minishell);
void				free_path(t_path *path);
void				free_env(t_env *env);
void				free_cmd_table(t_shell *minishell);
// free_memory_loop.c
void				reset_loop_rest(t_shell *minishell);
void				reset_loop(t_shell *minishell);
// init_arg.c
char				*get_line(t_mode mode, int fd);
int					ft_check_emptyline(char *line);
int					init_argc(int argc, char **argv, int fd);
// init_env_nodes.c
void				set_shlvl_inception(t_shell *minishell, t_env *node);
void				init_env_node(t_shell *minishell, t_env *node, char *str);
void				init_env_nodes(t_shell *minishell, t_env *node, char **envp,
						int i);
// init_no_env.c
void				set_oldpwd(t_shell *minishell);
void				set_last_command(t_shell *minishell);
void				set_shlvl(t_shell *minishell);
void				init_pwd(t_shell *minishell);
void				set_no_env(t_shell *minishell);
// init.c
void				fill_env(t_shell *minishell, char **envp);
void				fill_envp(t_shell *minishell);
char				*getpath(t_shell *minishell, char *key);
void				fill_path(t_shell *minishell);
void				init_minishell(t_shell *minishell, char **envp, int argc);
// signals.c
void				ft_handle_sig(int s);
void				ft_sig_heredoc(int s);
void				set_signal_heredoc(void);
int					ft_catchsignals(t_shell *minishell);
void				set_signals(void);
// utils.c
void				safe_write(int fd, const char *str, ...);
char				*safe_join_envp(char *key, char *symb, char *value);
void				ft_perror(t_shell *minishell, char *word, char *msg,
						char *third);
int					env_size(t_shell *minishell);
void				exitmsg(t_shell *minishell, char *errmsg);
// utils_libft.c
int					safe_strcmp(char *s1, char *s2);
void				swap_env(t_list *a, t_list *b);
///////////////////

/////////////////// Parseur expendeur llexeur
int					ft_parseur(t_shell *minishell);
void				ft_dollar_option(char *copy, t_shell *minishell, int i,
						int flag_dbl);
void				ft_expand_dollar(t_shell *minishell, int flag_sgl,
						int flag_dbl);
void				ft_rev_neg_line(t_shell *minishell);
char				*ft_strndup(char *str, int n);
void				ft_neg_inside_quote(t_shell *minishell);
int					ft_parseur_quote(t_shell *minishell);
char				*ft_jnf(char *s1, char *s2);
int					ft_strcmp(const char *s1, const char *s2);
void				ft_init_node_list(t_node **nodes,
						t_cmd_table *command_table, t_shell *minishell);
int					ft_rev_neg_words(char *line);
void				ft_tokenize_list(t_node *nodes, t_shell *minishell);
int				ft_check_syntax(t_node *nodes, t_shell *minishell);
void				ft_redistribute_node(t_cmd_table **command_table,
						t_node *nodes, t_shell *minishell);
void				ft_init_redir_list_in(t_redir **redir, t_node *nodes,
						t_shell *minishell);
void				ft_init_redir_list_out(t_redir **redir, t_node *nodes,
						t_shell *minishell);
void				ft_free_double_char(char **tab);
void				safe_free(char *string);
void				ft_free_redir(t_redir *redir_def);
void				ft_free_node(t_node *node_def);
// NODE INIT
t_node				*ft_new_node(char *word, int flag, t_shell *minishell);
t_redir				*ft_last_stack_redir(t_redir *a);
void				ft_stack_add_to_back(t_node **a, t_node *new_node);
///////////////////

#endif
