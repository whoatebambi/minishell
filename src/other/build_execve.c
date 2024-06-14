/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build_execve.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcouserg <fcouserg@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/21 20:27:20 by fcouserg          #+#    #+#             */
/*   Updated: 2024/06/14 17:01:24 by fcouserg         ###   ########.fr       */
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