/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initializing.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcouserg <fcouserg@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/21 20:27:20 by fcouserg          #+#    #+#             */
/*   Updated: 2024/09/20 16:08:29 by fcouserg         ###   ########.fr       */
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

t_shell	*init_minishell(char **envp, int argc)
{
	t_shell	*minishell;

	minishell = ft_calloc(sizeof(t_shell), 1);
    // TD protect ft_calloc here
	get_minishell(minishell);
    
	if (argc == 1)
		minishell->mode = INTERACTIVE;
	else
		minishell->mode = NON_INTERACTIVE;
	minishell->cmd_table = NULL;
    // TD can **envp be NULL? if yes what happens?
	minishell->env_lst = init_env_lst(envp);
    minishell->envp = NULL;
	minishell->child_pids = NULL;
	minishell->tmp_exit_code = 0;
	minishell->exit_code = 0;
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
	    env_var->var = ft_strdup(envp + i + 1);
    else
        env_var->var = NULL;
    // TD protect ft_strdup & ft_substr here
	return (env_var);
}