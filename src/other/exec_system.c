/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_system.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/21 20:27:20 by fcouserg          #+#    #+#             */
/*   Updated: 2024/10/06 21:52:56 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**get_execpath(t_shell *shell)
{
	t_env	*env;
	char	**execve_path;

	env = shell->env;
	execve_path = NULL;
	while (env)
	{
		if (safe_strcmp(env->key, "PATH") == 0)
		{
			execve_path = ft_split(env->value, ':');
			break;
		}
		env = env->next;
	}
	return (execve_path);
}

void	exec_system(char **tab, t_shell *minishell)
{
	char	**execve_path_table;
	
	if (ft_strncmp("/", tab[0], 1) == 0)
	{
		if (access(tab[0], F_OK) == 0 && access(tab[0], X_OK) == 0)
		{
			minishell->execve_path = ft_strdup(tab[0]);
			// return (0);
		}
		// return (FILE_NOT_FOUND);
	}
	else
	{
		execve_path_table = get_execpath(minishell);
		if (execve_path_table == NULL)
			exitmsg(minishell, MERROR);
		minishell->execve_path = find_relative_path(tab[0], execve_path_table);
		ft_free_double_char(execve_path_table);
	}
	if (minishell->execve_path == NULL)
		exitmsg(minishell, MERROR);
		// perror("access");
	execve(minishell->execve_path, tab, minishell->envp);
	perror("execve");
    exit(EXIT_FAILURE);
}

char	*find_relative_path(char *arg, char **envp)
{
	char	*execve_dup;
	char	*path_arg;
	int		i;

	i = 0;
	path_arg = ft_strjoin_no_free("/", arg);
	while (envp[i])
	{
		execve_dup = ft_strjoin_no_free(envp[i], path_arg);
		if (access(execve_dup, F_OK) == 0 && access(execve_dup, X_OK) == 0)
		{
			free(path_arg);
			return (execve_dup);
		}
		free(execve_dup);
		i++;
	}
	free(path_arg);
	return (NULL);
}