/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_children.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/21 20:27:20 by fcouserg          #+#    #+#             */
/*   Updated: 2024/11/05 01:05:55 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	exec_child_builtin(t_shell *mns, t_cmd_table *cmd, t_fds *fd, int i)
{
	if (fd->savedpipe != -42)
		close(fd->savedpipe);
	if (fd->pipes[0] != -42)
		close(fd->pipes[0]);
	if (fd->redir[0] != -42)
		if (dup2(fd->redir[0], STDIN_FILENO) == -1)
			(close_fds(fd), exitmsg(mns, NULL));
	if (fd->redir[1] != -42)
		if (dup2(fd->redir[1], STDOUT_FILENO) == -1)
			(close_fds(fd), exitmsg(mns, NULL));
	close_fds(fd);
	if (cmd->builtin == ENV)
		builtin_env(mns->env, mns);
	if (cmd->builtin == PWD)
		builtin_pwd(STDOUT_FILENO, mns);
	if (cmd->builtin == ECHO)
		builtin_echo(cmd->tab, mns, i);
}

void	handle_fd_error(t_shell *minishell, t_fds *fd)
{
	if (fd->pipes[0] != -42)
		close(fd->pipes[0]);
	if (fd->savedpipe != -42)
		close(fd->savedpipe);
	if (fd->redir[0] != -42)
		if (dup2(fd->redir[0], STDIN_FILENO) == -1)
			(close_fds(fd), exitmsg(minishell, NULL));
	if (fd->redir[1] != -42)
		if (dup2(fd->redir[1], STDOUT_FILENO) == -1)
			(close_fds(fd), exitmsg(minishell, NULL));
}

void	handle_path_error(t_shell *minishell, t_cmd_table *cmd, t_fds *fd)
{
	if (cmd->pathnoaccess)
	{
		if (!cmd->path)
			ft_perror(minishell, cmd->tab[0], strerror(errno), NULL);
		close_fds(fd);
		exitmsg(minishell, NULL);
	}
	else if (!cmd->path)
	{
		close_fds(fd);
		exitmsg(minishell, NULL);
	}
}

void	exec_child(t_shell *minishell, t_cmd_table *cmd, t_fds *fd)
{
	handle_fd_error(minishell, fd);
	handle_path_error(minishell, cmd, fd);
	close_fds(fd);
	if (execve(cmd->path, cmd->tab, minishell->envp) == -1)
		(ft_perror(minishell, cmd->path, "command not found", NULL),
			exitmsg(minishell, NULL));
}
