/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executing.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/21 20:27:20 by fcouserg          #+#    #+#             */
/*   Updated: 2024/10/23 13:41:08 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	execute_builtin(t_cmd_table *cmd_table, t_shell *minishell, t_fds *fd)
{
	if (safe_strcmp(cmd_table->tab[0], "pwd") == 0)
		pwd(STDOUT_FILENO, minishell);
    else if (safe_strcmp(cmd_table->tab[0], "cd") == 0)
		cd(cmd_table->tab, minishell->env, minishell);
	else if (safe_strcmp(cmd_table->tab[0], "echo") == 0)
		echo(cmd_table->tab, STDOUT_FILENO);
	else if (safe_strcmp(cmd_table->tab[0], "env") == 0)
		ft_env(minishell->env, STDOUT_FILENO);
	else if (safe_strcmp(cmd_table->tab[0], "export") == 0)
		export(minishell->env, cmd_table->tab, STDOUT_FILENO, minishell);
	else if (safe_strcmp(cmd_table->tab[0], "unset") == 0)
		unset(cmd_table->tab, minishell);
	else if (safe_strcmp(cmd_table->tab[0], "exit") == 0)
		ft_exit(cmd_table->tab, minishell, fd);
	return (1);
}

void	ft_exec(t_shell *minishell, int i, t_fds *fd)
{
	if (is_builtin(minishell->cmd_table[i]->tab[0]) == 1)
		execute_builtin(minishell->cmd_table[i], minishell, fd);
	else
	{
		minishell->child_pids[i] = fork();
		if (minishell->child_pids[i] == -1)
			exitmsg(minishell, "fork");
		if (minishell->child_pids[i] == 0)
		{
			if (fd->pipes[0] != -42)
				close(fd->pipes[0]);
			if (fd->redir[0] != -42)
				if (dup2(fd->redir[0], STDIN_FILENO) == -1)
					exitmsg(minishell, "dup2");
			if (fd->redir[1] != -42)		
				if (dup2(fd->redir[1], STDOUT_FILENO) == -1)
					exitmsg(minishell, "dup2 TEST_2");
			close_fds(fd);		
			if (is_builtin(minishell->cmd_table[i]->tab[0]) == 2)
			{
				// add flag for exit() if there is a cmd before or after?
				execute_builtin(minishell->cmd_table[i], minishell, fd);
				exit(minishell->excode);
			}
			else
				exec_system(minishell->cmd_table[i]->tab, minishell);
		}
		close_fds_parent(fd);
		fd->redir[0] = fd->pipes[0];	
	}
	
}

void	close_fds_parent(t_fds *fd)
{
	if (fd->redir[0] != -42 && fd->redir[0] >= 0)
		close(fd->redir[0]);
	if (fd->redir[1] != -42 && fd->redir[1] >= 0)
		close(fd->redir[1]);
}

void	ft_wait_all_children(t_shell *minishell)
{
	int		status;
	int i = 0;

	status = -1;
	
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


void	start_exec(t_shell *minishell)
{
	int		i;
	t_fds	fd;

	i = 0;
	if (minishell->count_pipes == 0 || minishell->cmd_table[0]->tab[0] == NULL || minishell->cmd_table[0]->tab[0][0] == '\n')
		return ;
	fd.redir[0] = -42;
	while (i < minishell->count_pipes)
	{
		minishell->child_pids[i] = -2;
		ft_init_fds(&fd);
		ft_pipes(minishell, &fd, i);
		handle_redirs(minishell, &fd, i);
		if (minishell->excode == 130)
			break ;
		set_redirs(&fd);
		// if (is_builtin(minishell->cmd_table[i]->tab[0]) && minishell->count_pipes == 1)
		// 	execute_builtin(minishell->cmd_table[i], minishell, &fd);
		// else if (is_builtin(minishell->cmd_table[i]->tab[0]) == 2)
		// 	execute_builtin(minishell->cmd_table[i], minishell, &fd);
		// else
		// 	ft_exec(minishell, i, &fd);
		ft_exec(minishell, i, &fd);
		if (i + 1 == minishell->count_pipes)
			close_fds_parent(&fd);
		i++;
	}
	ft_wait_all_children(minishell);
	close_fds(&fd);
}
