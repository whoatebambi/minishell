/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executing.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbeaudoi <gbeaudoi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/21 20:27:20 by fcouserg          #+#    #+#             */
/*   Updated: 2024/06/20 19:30:26 by gbeaudoi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	execute_builtin(t_cmd_table *cmd_table, t_list *env_lst, t_fds *fd)
{
    int exit_code;

	if (safe_strcmp(cmd_table->cmd_args[0], "pwd") == 0)
		exit_code = pwd(STDOUT_FILENO);
    else if (safe_strcmp(cmd_table->cmd_args[0], "cd") == 0)
		exit_code = cd(cmd_table->cmd_args, env_lst);
	else if (safe_strcmp(cmd_table->cmd_args[0], "echo") == 0)
		exit_code = echo(cmd_table->cmd_args, STDOUT_FILENO);
	else if (safe_strcmp(cmd_table->cmd_args[0], "env") == 0)
		env(env_lst, STDOUT_FILENO);
	else if (safe_strcmp(cmd_table->cmd_args[0], "export") == 0)
		export(env_lst, cmd_table, STDOUT_FILENO);
	return (1);
}

void	exec_in_child(t_shell *minishell, int i, t_fds *fd)
{
	minishell->child_pids[i] = fork();
	if (minishell->child_pids[i] == -1)
	{
		perror("minishell->child_pids[i]");
	}
	if (minishell->child_pids[i] == 0)
	{
		if (fd->pipes[0] != -42)
			close(fd->pipes[0]);
		if (fd->redir[0] != -42)
			if (dup2(fd->redir[0], STDIN_FILENO) == -1)
				exit(1);
		if (fd->redir[1] != -42)		
			if (dup2(fd->redir[1], STDOUT_FILENO) == -1)
				exit(1);
		close_fds(fd);		
		if (is_builtin(minishell->cmd_table[i]->cmd_args[0]))
			exit(execute_builtin(minishell->cmd_table[i], minishell->env_lst, fd));
		else
		{
			exec_system(minishell, i);
		}
	}
	close_fds_parent(fd);
	fd->redir[0] = fd->pipes[0];
}

void	close_fds_parent(t_fds *fd)
{
	if (fd->redir[0] != -42 && fd->redir[0] >= 0)
		close(fd->redir[0]);
	if (fd->redir[1] != -42 && fd->redir[1] >= 0)
		close(fd->redir[1]);
}

static void	ft_wait_all_children(t_shell *minishell)
{
	int		i;
	int		*exit_codes;
	int		status;
	pid_t	pid_wait;

	exit_codes = ft_calloc(sizeof(int), minishell->count_pipes);
	if (exit_codes == NULL)
	{
		// exit;
	}
	i = 0;
	while (i < minishell->count_pipes)
	{
		pid_wait = waitpid(minishell->child_pids[i], &status, 0);
		if (pid_wait > 0)
		{
			// printf("Parent: Child with PID %d terminated with status %d\n",
			// 	minishell->child_pids[i], WEXITSTATUS(status));
			// printf("Parent: Child with PID %d finished, i is %d\n",
			// minishell->child_pids[i], i);
		}
		if (WIFEXITED(status))
			exit_codes[i] = WEXITSTATUS(status);
		i++;
	}
	// if (minishell->exit_code != EXIT_SIGQUIT
	// 	&& minishell->exit_code != EXIT_SIGINT)
		minishell->exit_code = exit_codes[minishell->count_pipes - 1];
	free(exit_codes);
}

void	execute(t_shell *minishell)
{
	int		i;
	t_fds	fd;

	i = 0;
	fd.redir[0] = -42;
	while (i < minishell->count_pipes)
	{
		minishell->child_pids[i] = -2;
		ft_init_fds(&fd);
		// ft_printf("i is %d, pid is %d\n", i, minishell->child_pids[i]);
		// ft_printf("1: input %d output %d pipe0 %d pipe1 %d redir0 %d redir1 %d\n", fd.input, fd.output, fd.pipes[0], fd.pipes[1], fd.redir[0],fd.redir[1]);
		if (i < minishell->count_pipes - 1)
		{
			if (pipe(fd.pipes) == -1)
				ft_printf("ERROR");
		}
		// ft_printf("2: input %d output %d pipe0 %d pipe1 %d redir0 %d redir1 %d\n", fd.input, fd.output, fd.pipes[0], fd.pipes[1], fd.redir[0], fd.redir[1]);
		exec_redirs(minishell, &fd, i);
		// ft_printf("3: input %d output %d pipe0 %d pipe1 %d redir0 %d redir1 %d\n", fd.input, fd.output, fd.pipes[0], fd.pipes[1], fd.redir[0],fd.redir[1]);
		set_redirs(&fd);
		// ft_printf("4: input %d output %d pipe0 %d pipe1 %d redir0 %d redir1 %d\n", fd.input, fd.output, fd.pipes[0], fd.pipes[1], fd.redir[0],fd.redir[1]);
		if (is_builtin(minishell->cmd_table[i]->cmd_args[0])
			&& minishell->count_pipes == 1)
			execute_builtin(minishell->cmd_table[i], minishell->env_lst, &fd);
		else if (is_builtin(minishell->cmd_table[i]->cmd_args[0]) == 2)
			execute_builtin(minishell->cmd_table[i], minishell->env_lst, &fd);
		else
			exec_in_child(minishell, i, &fd);
		if (i + 1 == minishell->count_pipes)
			close_fds_parent(&fd);
		i++;
	}
	ft_wait_all_children(minishell);
	close_fds(&fd);
}

// echo papa >file1 | cat < file1