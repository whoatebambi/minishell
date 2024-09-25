/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executing.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcouserg <fcouserg@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/21 20:27:20 by fcouserg          #+#    #+#             */
/*   Updated: 2024/09/25 17:39:51 by fcouserg         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	execute_builtin(t_cmd_table *cmd_table, t_list *env_lst, t_fds *fd)
{
	if (safe_strcmp(cmd_table->cmd_args[0], "pwd") == 0)
		pwd(STDOUT_FILENO);
    else if (safe_strcmp(cmd_table->cmd_args[0], "cd") == 0)
		cd(cmd_table->cmd_args, env_lst);
	else if (safe_strcmp(cmd_table->cmd_args[0], "echo") == 0)
		echo(cmd_table->cmd_args, STDOUT_FILENO);
	else if (safe_strcmp(cmd_table->cmd_args[0], "envp") == 0)
		envp(env_lst, STDOUT_FILENO);
	else if (safe_strcmp(cmd_table->cmd_args[0], "export") == 0)
		export(env_lst, cmd_table->cmd_args, STDOUT_FILENO);
	return (1);
}

void	exec_in_child(t_shell *minishell, int i, t_fds *fd)
{
	minishell->child_pids[i] = fork();
	if (minishell->child_pids[i] == -1)
		perror("minishell->child_pids[i]");
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
			exec_system(minishell->cmd_table[i]->cmd_args, minishell);
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
	// if (exit_codes == NULL)
	// {
	// 	// exit;
	// }
	i = 0;
	while (i < minishell->count_pipes)
	{
		pid_wait = waitpid(minishell->child_pids[i], &status, 0);
		if (WIFEXITED(status))
			exit_codes[i] = WEXITSTATUS(status);
		i++;
	}
	// if (minishell->excode != EXIT_SIGQUIT
	// 	&& minishell->excode != EXIT_SIGINT)
	// 	minishell->excode = exit_codes[minishell->count_pipes - 1];
	free(exit_codes);
}

void	prep_exec(t_shell *minishell, t_fds *fd)
{
	int		i;
	int j;

	i = 0;
	fd->redir[0] = -42;
	while (i < minishell->count_pipes)
	{
		j = is_builtin(minishell->cmd_table[i]->cmd_args[0]);
		i++;
	}
	// minishell->tabpath = get_execpath(minishell);
	// handle_withpath(minishell, node, -1, 0);
}

void	start_exec(t_shell *minishell)
{
	int		i;
	t_fds	fd;

	i = 0;
	prep_exec(minishell, &fd);
	while (i < minishell->count_pipes)
	{
		minishell->child_pids[i] = -2;
		ft_init_fds(&fd);
		if (i < minishell->count_pipes - 1)
		{
			if (pipe(fd.pipes) == -1)
				perror("pipe");
				// ft_printf("ERROR");
		}
		exec_redirs(minishell, &fd, i);
		set_redirs(&fd);
		if (is_builtin(minishell->cmd_table[i]->cmd_args[0]) && minishell->count_pipes == 1)
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