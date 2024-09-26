/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/21 20:27:20 by fcouserg          #+#    #+#             */
/*   Updated: 2024/09/25 23:48:00 by codespace        ###   ########.fr       */
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
		fd = open(argv[1], O_RDONLY);
	if (fd == -1)
	{
		// no need to set errno to a macro here, the system call open() automatically did it
		perror("Error");
		exit(EXIT_FAILURE);
	}
    return (fd);
}

void    ft_getenv(t_shell *minishell, char **envp)
{
    t_env	*new_node;
	int		i;

	i = 0;
	if (!envp || !envp[i])
	{
		ft_no_env(minishell);
		return ;
	}
	while (envp && envp[i])
	{
		new_node = malloc(sizeof(t_env));
		// if (!new_node)
			// exitmsg(shell, MERROR);
		init_env_nodes(minishell, new_node, envp, i);
		i++;
	}
}

char	**build_execve_envp(t_list *env_lst)
{
	char	**execve_envp;
	int		count;
	int 	i;
	
	count = ft_lstsize(env_lst);
	i = 0;
	execve_envp = (char **)malloc(sizeof(char *) * (count + 1));
	while (env_lst)
	{
		execve_envp[i] = ft_strjoin_no_free(((t_env *)(env_lst->content))->key, "=");
		execve_envp[i] = ft_strjoin(execve_envp[i], ((t_env *)(env_lst->content))->value);
		env_lst = env_lst->next;
		i++;
	}
	execve_envp[i] = NULL;
	return (execve_envp);
}

int	env_size(t_shell *shell)
{
	t_env	*node;
	int		len;

	len = 0;
	node = shell->env;
	while (node)
	{
		if (node->key)
			len++;
		node = node->next;
	}
	return (len);
}

void	fill_envp(t_shell *minishell)
{
	int		len;
	int		i;
	t_env	*curr;

	i = 0;
	len = env_size(minishell);
	curr = minishell->env;
	minishell->envp = ft_calloc(len + 1, sizeof(char *));
	// if (!minishell->envp)
	// 	exitmsg(minishell, MERROR);
	while (curr)
	{
		if (curr->key && !curr->isunset) // isunset????
		{
			minishell->envp[i] = ft_strjoin_no_free(curr->key, "=");
			minishell->envp[i] = ft_strjoin(minishell->envp[i], curr->value);
			// if (!minishell->envp[i])
			// 	exitmsg(minishell, MERROR);
			i++;
		}
		curr = curr->next;
	}
}

char	*getpath(t_shell *minishell, char *key)
{
	t_env	*envnode;

	envnode = minishell->env;
	while (envnode)
	{
		if (safe_strcmp(envnode->key, key) == 0) // ou l'inverse ?
			break ;
		envnode = envnode->next;
	}
	if (!envnode)
		return (NULL);
	return (envnode->value);
}

t_shell	*init_minishell(t_shell	*minishell, char **envp, int argc)
{
	char *path;
	
	if (argc == 1)
		minishell->mode = INTERACTIVE;
	else
		minishell->mode = NON_INTERACTIVE;
	minishell->env = NULL;
	ft_getenv(minishell, envp);
	minishell->envp = NULL;
	fill_envp(minishell);
    minishell->env_lst = init_env_lst(envp);   // a supprimer 
	minishell->path = ft_calloc(1, sizeof(t_path));
	// if (!shell->path)
	// 	exitmsg(shell, MERROR);
	path = getpath(minishell, "PWD");
	if (path)
	{
		minishell->path->pwd = ft_strdup(path);
		// if (!shell->path->pwd)
		// 	exitmsg(shell, MERROR);
	}
	path = getpath(minishell, "OLDPWD");
	if (path)
	{
		minishell->path->oldpwd = ft_strdup(path);
		// if (!shell->path->oldpwd)
		// 	exitmsg(shell, MERROR);
	}
	minishell->lex = NULL;
    minishell->cmd = NULL;
    // minishell->tabpath = build_execve_path(minishell->env_lst);
	minishell->cmd_table = NULL;
    minishell->child_pids = NULL;
	minishell->tmpexcode = 0;
	minishell->excode = 0;
    minishell->line = NULL;
    minishell->clean_line = NULL;
	minishell->inflagerr = 0;
	minishell->outflagerr = 0;
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