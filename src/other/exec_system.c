/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_system.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/21 20:27:20 by fcouserg          #+#    #+#             */
/*   Updated: 2024/09/25 23:50:29 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	exec_system(char **cmd_args, t_shell *minishell)
{
	char	*execve_path;
	
	if (ft_strncmp("/", cmd_args[0], 1) == 0)
		execve_path = cmd_args[0];
	else
		execve_path = find_relative_path(cmd_args[0], minishell->tabpath);
	if (execve_path == NULL)
		perror("access");
	// execve(execve_path, cmd_args, minishell->envp);
	if (execve(execve_path, cmd_args, minishell->envp) == -1)
		perror("execve");
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