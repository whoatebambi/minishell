// /* ************************************************************************** */
// /*                                                                            */
// /*                                                        :::      ::::::::   */
// /*   exec_system.c                                      :+:      :+:    :+:   */
// /*                                                    +:+ +:+         +:+     */
// /*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
// /*                                                +#+#+#+#+#+   +#+           */
// /*   Created: 2024/02/21 20:27:20 by fcouserg          #+#    #+#             */
// /*   Updated: 2024/10/25 15:56:48 by codespace        ###   ########.fr       */
// /*                                                                            */
// /* ************************************************************************** */

// #include "minishell.h"

// char	**get_execpath(t_shell *shell)
// {
// 	t_env	*env;
// 	char	**execve_path;

// 	env = shell->env;
// 	execve_path = NULL;
// 	while (env)
// 	{
// 		if (safe_strcmp(env->key, "PATH") == 0)
// 		{
// 			execve_path = ft_split(env->value, ':');
// 			break;
// 		}
// 		env = env->next;
// 	}
// 	return (execve_path);
// }

// char	*find_relative_path(char *arg, char **envp)
// {
// 	char	*execve_dup;
// 	char	*path_arg;
// 	int		i;

// 	i = 0;
// 	path_arg = ft_strjoin_no_free("/", arg);
// 	while (envp[i])
// 	{
// 		execve_dup = ft_strjoin_no_free(envp[i], path_arg);
// 		if (access(execve_dup, X_OK) == 0 && access(execve_dup, F_OK) == 0) 
// 		{
// 			free(path_arg);
// 			return (execve_dup);
// 		}
// 		free(execve_dup);
// 		i++;
// 	}
// 	free(path_arg);
// 	return (NULL);
// }

// void	exec_system(char **tab, t_shell *minishell)
// {
// 	char	**execve_path_table;
// 	char *dup;
// 	struct stat	info;

// 	if (ft_strchr(tab[0], '/'))
// 	{
// 		if (access(tab[0], F_OK) == 0)
// 		{
// 			if (stat(tab[0], &info) == 0 && S_ISDIR(info.st_mode))
// 			{
// 				safe_write(2, "minishell: ", tab[0], ": Is a directory\n", NULL);
// 				minishell->excode = 126;
// 				return ;
// 			}
// 			if (access(tab[0], X_OK) == -1)
// 			{
// 				safe_write(2, "minishell: ", tab[0], ": Permission denied\n", NULL);
// 				minishell->excode = 126;
// 				return ;
// 			}
// 			minishell->execve_path = ft_strdup(tab[0]);
// 		}
// 		else
// 		{
// 			if (access(tab[0], F_OK) == -1)
// 			{
// 				safe_write(2, "minishell: ", tab[0], ": No such file or directory TEEEEEEEEEST\n", NULL);
// 				minishell->excode = 127;
// 				return ;
// 			}
// 		}
// 	}
// 	else
// 	{
// 		execve_path_table = get_execpath(minishell);
// 		if (execve_path_table == NULL)
// 			exitmsg(minishell, MERROR);// change error type
// 		minishell->execve_path = find_relative_path(tab[0], execve_path_table);
// 		ft_free_double_char(execve_path_table);
// 	}
// 	if (minishell->execve_path == NULL)
// 	{
		
// 		if (access(tab[0], F_OK) == 0 && access(tab[0], X_OK) == -1)
// 		{
// 			safe_write(2, "minishell: ", tab[0], ": Permission denied\n", NULL);
// 			minishell->excode = 126;
// 			return ;
// 		}		
// 		minishell->excode = 127;
// 		dup = ft_strndup(tab[0], ft_strlen(tab[0]));
// 		safe_write(2, dup, ": command not found\n", NULL);
// 		free(dup);
// 		exitmsg(minishell, NULL);
// 	}
// 	execve(minishell->execve_path, tab, minishell->envp);
// 	// perror(tab[0]);
// 	exit(EXIT_FAILURE);
// }

