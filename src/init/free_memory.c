/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_memory.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/21 20:27:20 by fcouserg          #+#    #+#             */
/*   Updated: 2024/11/05 02:09:40 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_cmd_table(t_shell *minishell)
{
	int	i;

	i = 0;
	if (minishell->cmd_table == NULL)
		return ;
	while (i < minishell->count_pipes)
	{
		if (minishell->cmd_table[i])
		{
			safe_free(minishell->cmd_table[i]->group_command);
			safe_free(minishell->cmd_table[i]->path);
			ft_free_node(minishell->cmd_table[i]->nodes);
			ft_free_double_char(minishell->cmd_table[i]->tab);
			ft_free_redir(minishell->cmd_table[i]->redirs_in);
			ft_free_redir(minishell->cmd_table[i]->redirs_out);
			safe_free(minishell->cmd_table[i]->infile_tmp);
			free(minishell->cmd_table[i]);
			minishell->cmd_table[i] = NULL;
		}
		i++;
	}
	free(minishell->cmd_table);
	minishell->cmd_table = NULL;
}

void	free_env(t_env *env)
{
	t_env	*tmp;

	if (env == NULL)
		return ;
	while (env)
	{
		tmp = env->next;
		if (env->key)
		{
			free(env->key);
			env->key = NULL;
		}
		if (env->value)
		{
			free(env->value);
			env->value = NULL;
		}
		free(env);
		env = tmp;
	}
}

void	free_path(t_path *path)
{
	if (path == NULL)
		return ;
	if (path->pwd)
	{
		free(path->pwd);
		path->pwd = NULL;
	}
	if (path->oldpwd)
	{
		free(path->oldpwd);
		path->oldpwd = NULL;
	}
	free(path);
}

void	free_minishell(t_shell *minishell)
{
	if (minishell == NULL)
		return ;
	reset_loop(minishell);
	free_env(minishell->env);
	free_path(minishell->path);
	if (minishell->envp)
	{
		ft_free_double_char(minishell->envp);
		minishell->envp = NULL;
	}
}

void	free_specific_env(t_env	*new, char *str, t_shell *minishell)
{
	free(str);
	free(new);
	exitmsg(minishell, "Malloc error");
}
