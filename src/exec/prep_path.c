/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prep_path.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbeaudoi <gbeaudoi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/21 20:27:20 by fcouserg          #+#    #+#             */
/*   Updated: 2024/11/05 11:41:37 by gbeaudoi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	path_true_access(t_shell *mns, int *i)
{
	struct stat	info;

	if (stat(mns->cmd_table[*i]->tab[0], &info) == 0 && S_ISDIR(info.st_mode))
	{
		ft_perror(mns, mns->cmd_table[*i]->tab[0], "Is a directory", NULL);
		mns->excode = 126;
		mns->cmd_table[*i]->pathnoaccess = true;
	}
	mns->cmd_table[*i]->path = ft_strdup(mns->cmd_table[*i]->tab[0]);
	if (!mns->cmd_table[*i]->path)
		exitmsg(mns, "Malloc error");
	*i = *i + 1;
}

int	path_true(t_shell *minishell, int *i)
{
	if (ft_strchr(minishell->cmd_table[*i]->tab[0], '/')
		&& access(minishell->cmd_table[*i]->tab[0], F_OK | X_OK) != 0)
	{
		minishell->excode = 127;
		minishell->cmd_table[*i]->pathnoaccess = true;
		*i = *i + 1;
		return (1);
	}
	else if (ft_strchr(minishell->cmd_table[*i]->tab[0], '/')
		&& access(minishell->cmd_table[*i]->tab[0], F_OK | X_OK) == 0)
	{
		path_true_access(minishell, i);
		return (1);
	}
	return (0);
}

void	set_path_str(t_shell *minishell, int *flag, int i, int j)
{
	char	*tmp1;
	char	*tmp2;

	tmp1 = ft_jnf("/", minishell->cmd_table[i]->tab[0]);
	tmp2 = ft_jnf(minishell->tabpath[j], tmp1);
	free(tmp1);
	tmp1 = NULL;
	if (!tmp2)
		exitmsg(minishell, "Malloc error");
	if (access(tmp2, X_OK) == 0)
		set_cmdtable_path(minishell, minishell->cmd_table[i], tmp2, flag);
	free(tmp2);
	tmp2 = NULL;
}

void	set_path_true(t_shell *minishell, int flag)
{
	int	i;
	int	j;

	i = 0;
	while (i < minishell->count_pipes)
	{
		j = -1;
		if (minishell->cmd_table[i]->tab[0])
		{
			if (path_true(minishell, &(i)))
				continue ;
			flag = 0;
			while (minishell->tabpath && minishell->tabpath[++j])
				set_path_str(minishell, &flag, i, j);
			if (!flag && minishell->cmd_table[i]->builtin == NOT)
				set_cmdtable_path_still(minishell, minishell->cmd_table[i]);
		}
		i++;
	}
}
