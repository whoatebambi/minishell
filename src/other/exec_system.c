/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_system.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcouserg <fcouserg@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/21 20:27:20 by fcouserg          #+#    #+#             */
/*   Updated: 2024/06/21 19:32:28 by fcouserg         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
		execve_envp[i] = ft_strjoin(execve_envp[i], ((t_env *)(env_lst->content))->var);
		env_lst = env_lst->next;
		i++;
	}
	execve_envp[i] = NULL;
	return (execve_envp);
}

char	**build_execve_path(t_list *env_lst)
{
	char	**execve_path_table;
	char	*path_to_split;
	int		i;
	
	i = 0;
	execve_path_table = NULL;
	path_to_split = NULL;
	while (env_lst)
	{
		if (ft_strncmp(((t_env *)(env_lst->content))->key, "PATH", 4) == 0)
		{
			execve_path_table = ft_split(((t_env *)(env_lst->content))->var, ':');
			break;
		}
		env_lst = env_lst->next;
	}
	return (execve_path_table);
}

void	exec_system(char **cmd_args, t_list *env_lst)
{
	char	**execve_envp;
	char	**execve_path_table;
	char	*execve_path;
	
	execve_envp = build_execve_envp(env_lst);
	execve_path_table = build_execve_path(env_lst);
	
	if (ft_strncmp("/", cmd_args[0], 1) == 0)
		execve_path = cmd_args[0];
	else
		execve_path = find_relative_path(cmd_args[0], execve_path_table);
	// if (execve_path == NULL)
	// 	perror("access");
	// ft_free_double_char(execve_envp);
	ft_free_double_char(execve_path_table);
	execve(execve_path, cmd_args, execve_envp);
	//perror("execve");
}

char	*find_relative_path(char *arg, char **execve_path_table)
{
	char	*execve_path;
	char	*path_arg;
	int		i;

	i = 0;
	path_arg = ft_strjoin_no_free("/", arg);
	while (execve_path_table[i])
	{
		execve_path = ft_strjoin_no_free(execve_path_table[i], path_arg);
		if (access(execve_path, F_OK | X_OK) == 0)
		{
			free(path_arg);
			return (execve_path);
		}
		free(execve_path);
		i++;
	}
	free(path_arg);
	execve_path = NULL;
	return (execve_path);
}