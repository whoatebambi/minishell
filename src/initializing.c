/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initializing.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcouserg <fcouserg@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/21 20:27:20 by fcouserg          #+#    #+#             */
/*   Updated: 2024/05/28 16:56:15 by fcouserg         ###   ########.fr       */
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

t_env	*add_env_var(char *envp)
{
	t_env	*env_var;
    int i;

    i = 0;
    while (envp[i] != '=' && envp[i])
        i++;    
    env_var = ft_calloc(sizeof(t_env), 1);
    if (!env_var)
        return (NULL);
	env_var->key = ft_substr(envp, 0, i);
    // TD manage SHLVH levels here
    if (envp[i])
	    env_var->var = ft_strdup(envp + i + 1);
    else
        env_var->var = NULL;
    // TD protect ft_strdup & ft_substr here
	return (env_var);
}

t_list  *init_env_lst(char **envp)
{
    t_list  *env_lst;
    t_env   *env_var;
    int     i;

	i = 0;
	env_lst = NULL;
	while (envp[i])
	{
		env_var = add_env_var(envp[i]);
		ft_lstadd_back(&env_lst, ft_lstnew((void *)env_var));
		i++;
	}
    return (env_lst);
}