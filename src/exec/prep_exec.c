/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prep_exec.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/21 20:27:20 by fcouserg          #+#    #+#             */
/*   Updated: 2024/11/05 01:10:49 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	set_pipes(t_shell *minishell, t_fds *fd, int i)
{
	if (i > 0 && minishell->cmd_table[i - 1] != NULL)
		fd->prevpipe = true;
	else
		fd->prevpipe = false;
	if (i < minishell->count_pipes - 1)
	{
		if (pipe(fd->pipes) == -1)
			exitmsg(minishell, "pipe");
	}
}

void	check_builtins(t_cmd_table *cmd)
{
	if (safe_strcmp(cmd->tab[0], "echo") == 0)
		cmd->builtin = ECHO;
	if (safe_strcmp(cmd->tab[0], "cd") == 0)
		cmd->builtin = CD;
	if (safe_strcmp(cmd->tab[0], "pwd") == 0)
		cmd->builtin = PWD;
	if (safe_strcmp(cmd->tab[0], "export") == 0)
		cmd->builtin = EXPORT;
	if (safe_strcmp(cmd->tab[0], "unset") == 0)
		cmd->builtin = UNSET;
	if (safe_strcmp(cmd->tab[0], "env") == 0)
		cmd->builtin = ENV;
	if (safe_strcmp(cmd->tab[0], "exit") == 0)
		cmd->builtin = EXIT;
}

void	init_exec(t_shell *minishell, t_fds *fd)
{
	t_env	*env;
	int		i;

	fd->redir[0] = -42;
	minishell->tabpath = NULL;
	i = 0;
	env = minishell->env;
	while (i < minishell->count_pipes)
	{
		check_builtins(minishell->cmd_table[i]);
		i++;
	}
	while (env)
	{
		if (safe_strcmp(env->key, "PATH") == 0)
		{
			minishell->tabpath = ft_split(env->value, ':');
			break ;
		}
		env = env->next;
	}
	set_path_true(minishell, 0);
}
