/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/21 20:27:20 by fcouserg          #+#    #+#             */
/*   Updated: 2024/11/05 01:59:06 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	fill_env(t_shell *minishell, char **envp)
{
	t_env	*new_node;
	int		i;

	i = 0;
	if (!envp || !envp[i])
	{
		set_no_env(minishell);
		return ;
	}
	while (envp && envp[i])
	{
		new_node = malloc(sizeof(t_env));
		if (!new_node)
			exitmsg(minishell, "Malloc error");
		init_env_nodes(minishell, new_node, envp, i);
		i++;
	}
}

void	fill_envp(t_shell *minishell)
{
	int		len;
	int		i;
	t_env	*curr;

	i = 0;
	len = env_size(minishell);
	curr = minishell->env;
	if (minishell->envp)
		ft_free_double_char(minishell->envp);
	minishell->envp = ft_calloc(len + 1, sizeof(char *));
	if (!minishell->envp)
		exitmsg(minishell, "Malloc error");
	while (curr)
	{
		if (curr->key && !curr->isunset)
		{
			minishell->envp[i] = safe_join_envp(curr->key, "=", curr->value);
			if (minishell->envp[i] == NULL)
				exitmsg(minishell, "Malloc error");
			i++;
		}
		curr = curr->next;
	}
}

char	*getpath(t_shell *minishell, char *key)
{
	t_env	*env;

	env = minishell->env;
	while (env)
	{
		if (safe_strcmp(env->key, key) == 0)
			break ;
		env = env->next;
	}
	if (!env)
		return (NULL);
	return (env->value);
}

void	fill_path(t_shell *minishell)
{
	char	*str;

	minishell->path = ft_calloc(1, sizeof(t_path));
	if (!minishell->path)
		exitmsg(minishell, "Malloc error");
	str = getpath(minishell, "PWD");
	if (str)
	{
		minishell->path->pwd = ft_strdup(str);
		if (!minishell->path->pwd)
		{
			free(str);
			exitmsg(minishell, "Malloc error");
		}
	}
	str = getpath(minishell, "OLDPWD");
	if (str)
	{
		minishell->path->oldpwd = ft_strdup(str);
		if (!minishell->path->oldpwd)
		{
			free(str);
			exitmsg(minishell, "Malloc error");
		}
	}
}

void	init_minishell(t_shell	*minishell, char **envp, int argc)
{
	if (argc == 1)
		minishell->mode = INTERACTIVE;
	else
		minishell->mode = NON_INTERACTIVE;
	minishell->env = NULL;
	fill_env(minishell, envp);
	minishell->envp = NULL;
	fill_envp(minishell);
	minishell->path = NULL;
	fill_path(minishell);
	minishell->cmd_table = NULL;
	minishell->child_pids = NULL;
	minishell->count_pipes = 0;
	minishell->tmpexcode = 0;
	minishell->excode = 0;
	minishell->line = NULL;
	minishell->clean_line = NULL;
	minishell->execve_path = NULL;
	minishell->inflagerr = 0;
	minishell->outflagerr = 0;
	minishell->tabpath = NULL;
	minishell->inp = NULL;
	minishell->newinp = NULL;
	minishell->finalinp = NULL;
}
