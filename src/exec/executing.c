/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executing.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbeaudoi <gbeaudoi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/21 20:27:20 by fcouserg          #+#    #+#             */
/*   Updated: 2024/11/05 14:01:52 by gbeaudoi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	execute(t_shell *minishell, t_cmd_table *cmd, int i, t_fds *fd)
{
	if (cmd->builtin == EXIT)
		builtin_exit(cmd->tab, minishell, fd);
	else if (cmd->builtin == CD)
		builtin_cd(cmd->tab, minishell->env, minishell);
	else if (cmd->builtin == EXPORT)
		builtin_export(minishell->env, cmd->tab, minishell);
	else if (cmd->builtin == UNSET)
		builtin_unset(cmd->tab, minishell);
	else
	{
		minishell->child_pids[i] = fork();
		if (minishell->child_pids[i] == -1)
			exitmsg(minishell, "fork");
		if (minishell->child_pids[i] == 0)
		{
			if (cmd->builtin != NOT)
				exec_child_builtin(minishell, cmd, fd, i);
			else
				exec_child(minishell, cmd, fd);
		}
		fds_parent_close(fd);
		fd->redir[0] = fd->pipes[0];
	}
}

void	fds_parent_close(t_fds *fd)
{
	if (fd->redir[0] != -42 && fd->redir[0] >= 0)
		close(fd->redir[0]);
	if (fd->redir[1] != -42 && fd->redir[1] >= 0)
		close(fd->redir[1]);
}

void	wait_for_children(t_shell *minishell)
{
	int		status;
	int		i;

	status = -1;
	i = 0;
	while (i < minishell->count_pipes)
	{
		if (minishell->child_pids[i] != -2 && minishell->child_pids[i] != -1)
		{
			waitpid(minishell->child_pids[i], &status, 0);
			if (WIFEXITED(status))
				minishell->excode = WEXITSTATUS(status);
			if (errno == EACCES)
				minishell->excode = 126;
			if (minishell->child_pids[i] == -1)
				minishell->excode = 127;
		}
		i++;
	}
}

void	launch_exec(t_shell *minishell)
{
	int		i;
	t_fds	fd;

	i = 0;
	init_exec(minishell, &fd);
	while (i < minishell->count_pipes)
	{
		minishell->child_pids[i] = -2;
		init_struct_fd(&fd);
		set_pipes(minishell, &fd, i);
		init_redirections(minishell, &fd, i);
		if (minishell->excode == 130)
			break ;
		set_redirs(&fd);
		if (!minishell->cmd_table[i]->path
			&& minishell->cmd_table[i]->builtin == NOT)
			close_fds(&fd);
		if (minishell->cmd_table[i]->tab[0])
			execute(minishell, minishell->cmd_table[i], i, &fd);
		if (i + 1 == minishell->count_pipes)
			fds_parent_close(&fd);
		i++;
	}
	wait_for_children(minishell);
	close_fds(&fd);
}
