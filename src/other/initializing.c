/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initializing.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcouserg <fcouserg@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/21 20:27:20 by fcouserg          #+#    #+#             */
/*   Updated: 2024/09/25 19:01:21 by fcouserg         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	init_argc(int argc, char **argv, int fd)
{
	fd = 0;
	if (argc > 2) // usage is either "./minishell" or "./minishell <file>"
	{
		errno = E2BIG; // the int errno is set to the macro "E2BIG"
		perror("Error");
		exit(E2BIG); // "E2BIG" macro from <errno.h> expands to "Argument list too long."
	}
		
	if (argc == 2) // opens the file (we are in non-interactive mode)
	{
		fd = open(argv[1], O_RDONLY);
	}
	if (fd == -1)
	{
		// no need to set errno to a macro here, the system call open() automatically did it
		perror("Error");
		exit(EXIT_FAILURE);
	}
    return (fd);
}


	// char			*cwd;
	// char			**envp;
	// char			**tabpath;
	// t_path			*path_char;
	// t_env			*env;
	// t_lex			*lex;
	// t_cmd			*cmd;
	// int				excode;
	// int				tmpexcode;
	// char			*inp;
	// char			*newinp;
	// char			*finalinp;
	// int				inflagerr;
	// int				outflagerr;

	// t_mode			mode;
	// char			**envp;
	// char			**tabpath;
	// t_path			*path;
	// char			*path_char; //
	// t_env			*env;
	// t_list			*env_lst; //
	// t_lex			*lex;
	// t_cmd_table		**cmd_table; //
	// t_cmd			*cmd;
	// pid_t			*child_pids; //
	// int				excode;	
	// int				tmpexcode;
	// char			*inp;
	// char			*newinp;
	// char			*finalinp;
	// int				inflagerr;
	// int				outflagerr;
	// char			*line;
	// char			*clean_line;
	// int				count_pipes;



char    **init_envp(t_shell *minishell, char **envp)
{
    char    **envp_new;
    t_env	*newnode;
	int		i;

	i = 0;
	minishell->tmpexcode = 0;
	minishell->inflagerr = 0;
	minishell->outflagerr = 0;
	if (!envp || !envp[i])
	{
		ft_no_env(minishell);
		return ;
	}
	while (envp && envp[i])
	{
		newnode = malloc(sizeof(t_env));
		if (!newnode)
			exitmsg(shell, MERROR);
		init_env_nodes(shell, newnode, envp, i);
		i++;
	}
    return (envp_new);
}

t_shell	*init_minishell(t_shell	*minishell, char **envp, int argc)
{
	if (argc == 1)
		minishell->mode = INTERACTIVE;
	else
		minishell->mode = NON_INTERACTIVE;
        
    // shell->path = ft_calloc(1, sizeof(t_path));
	// if (!shell->path)
	// 	exitmsg(shell, MERROR);
        
    minishell->envp = NULL;
    minishell->envp = init_envp(minishell, envp);
    
	minishell->lex = NULL;
    minishell->cmd = NULL;
    minishell->env_lst = init_env_lst(envp);    
    
    minishell->tabpath = build_execve_path(minishell->env_lst);
	minishell->cmd_table = NULL;
    minishell->child_pids = NULL;
	minishell->tmpexcode = 0;
	minishell->excode = 0;
    minishell->line = NULL;
    minishell->clean_line = NULL;
	return (minishell);
}

int	init_fd(int argc, char **argv, int fd)
{
	fd = 0;
	if (argc == 2)
		fd = open(argv[1], O_RDONLY);
	if (fd == -1)
	{
		perror("Error");
		exit(EXIT_FAILURE);
	}
    return (fd);
}

char    *find_oldpwd(char **envp)
{
    char *pwd_path;
    int i;

    i = 0;
    pwd_path = NULL;
    while (envp[i])
    {
        if (ft_strncmp(envp[i], "PWD=", 4) == 0)
            pwd_path = ft_strdup(envp[i] + 4);
        if (ft_strncmp(envp[i], "OLDPWD=", 7) == 0)
        {
            if (pwd_path)
                free(pwd_path);
            return (NULL);
        }
        i++;
    }
    return (pwd_path);
}

char    **add_oldpwd(char **envp)
{
    char **envp_new;
    char *pwd_path;
    int i;
    int len;

    i = 0;
    len = 0;
    pwd_path = find_oldpwd(envp);
    if (pwd_path == NULL)
        return (envp);
    while (envp[len])
        len++;
    envp_new = (char **)ft_calloc(sizeof(char *), len + 2);
    while (envp[i])
    {
        envp_new[i] = ft_strdup(envp[i]);
        i++;
    }
    envp_new[i] = ft_strjoin_no_free("OLDPWD=", pwd_path);
    envp_new[i + 1] = NULL;
    free(pwd_path);
    return (envp_new);
}

t_list  *init_env_lst(char **envp)
{
    t_list  *env_lst;
    t_list  *new;
    t_env   *env_var;
    int     i;
    char **envp_new;

	i = 0;
	env_lst = NULL;
    env_var = NULL;
    if (!envp)
        return (NULL); // TD is this the right way to handle this?
    envp_new = add_oldpwd(envp); // check to make sure OLDPWD exists or creates it
	while (envp_new[i])
	{
		env_var = add_env_var(envp_new[i]);
        if (!env_var)
            return (NULL);
        new = ft_lstnew((void *)env_var);
        if (!new)
            return (NULL);
		ft_lstadd_back(&env_lst, new);
		i++;
	}
    return (env_lst);
}

t_env	*add_env_var(char *envp)
{
	t_env	*env_var;
    int i;

    if (!envp)
        return (NULL);
    i = 0;
    env_var = ft_calloc(sizeof(t_env), 1);
    if (!env_var)
        return (NULL);
    while (envp[i] != '=' && envp[i])
        i++;
	env_var->key = ft_substr(envp, 0, i);
    // TD manage SHLVH levels here
    if (envp[i])
	    env_var->value = ft_strdup(envp + i + 1);
    else
        env_var->value = NULL;
    // TD protect ft_strdup & ft_substr here
	return (env_var);
}