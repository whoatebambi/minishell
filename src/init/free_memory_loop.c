/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_memory_loop.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbeaudoi <gbeaudoi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/21 20:27:20 by fcouserg          #+#    #+#             */
/*   Updated: 2024/11/05 12:15:31 by gbeaudoi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	reset_loop_rest(t_shell *minishell)
{
	if (minishell->inp)
	{
		free(minishell->inp);
		minishell->inp = NULL;
	}
	if (minishell->newinp)
	{
		free(minishell->newinp);
		minishell->newinp = NULL;
	}
	if (minishell->finalinp)
	{
		free(minishell->finalinp);
		minishell->finalinp = NULL;
	}
	minishell->tmpexcode = minishell->excode;
	minishell->excode = 0;
	if (minishell->execve_path)
	{
		free(minishell->execve_path);
		minishell->execve_path = NULL;
	}
}

void	reset_loop(t_shell *minishell)
{
	if (minishell->line)
	{
		free(minishell->line);
		minishell->line = NULL;
	}
	if (minishell->clean_line)
	{
		free(minishell->clean_line);
		minishell->clean_line = NULL;
	}
	if (minishell->cmd_table)
		free_cmd_table(minishell);
	if (minishell->child_pids)
	{
		free(minishell->child_pids);
		minishell->child_pids = NULL;
	}
	if (minishell->tabpath)
	{
		ft_free_double_char(minishell->tabpath);
		minishell->tabpath = NULL;
	}
	reset_loop_rest(minishell);
}
