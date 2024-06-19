/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executing.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbeaudoi <gbeaudoi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/21 20:27:20 by fcouserg          #+#    #+#             */
/*   Updated: 2024/06/17 18:10:54 by gbeaudoi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	execute_builtin(t_cmd_table *cmd_table, t_list *env_lst)
{
    int exit_code;

	if (safe_strcmp(cmd_table->cmd_args[0], "pwd") == 0)
		exit_code = pwd(cmd_table->fd_out);
    else if (safe_strcmp(cmd_table->cmd_args[0], "cd") == 0)
		exit_code = cd(cmd_table->cmd_args, env_lst);
	else if (safe_strcmp(cmd_table->cmd_args[0], "echo") == 0)
		exit_code = echo(cmd_table->cmd_args, cmd_table->fd_out);
	else if (safe_strcmp(cmd_table->cmd_args[0], "env") == 0)
		env(env_lst, cmd_table->fd_out);
	else if (safe_strcmp(cmd_table->cmd_args[0], "export") == 0)
		export(env_lst, cmd_table, cmd_table->fd_out);
}

void	exec_in_child(t_shell *minishell, int i)
{
	pid_t	pid1;
	pid_t	pid_wait;
	int		status;

	// fork() here
	// set signals
	pid1 = fork();
	if (pid1 == -1)
	{
		perror("pid1");
	}
	else if (pid1 == 0)
	{
		// set fds
		if (minishell->cmd_table[i]->fd_in != 0)
		{
			dup2(minishell->cmd_table[i]->fd_in, STDIN_FILENO);
			close(minishell->cmd_table[i]->fd_in);
		}
		if (minishell->cmd_table[i]->fd_out != 1)
		{
			dup2(minishell->cmd_table[i]->fd_out, STDOUT_FILENO);
			close(minishell->cmd_table[i]->fd_out);
		}
		// printf("///// exec_in_child()\n");
		if (is_builtin(minishell->cmd_table[i]->cmd_args[0]))
			execute_builtin(minishell->cmd_table[i], minishell->env_lst);
		else
			exec_system(minishell, i);
	}
	// else
	// {
	pid_wait = waitpid(pid1, &status, 0);
	if (pid_wait > 0)
	{
		// printf("Parent: Child with PID %d terminated with status %d\n",
		// 	pid1, WEXITSTATUS(status));
		printf("Parent: Child with PID %d finished, i is %d\n", pid1, i);
	}
	// }
}

void	ft_set_pipe(t_shell *minishell, int i)
{
	if (pipe(minishell->cmd_table[i]->fd_pipe) == -1)
	{
		perror("pipe");
		exit(1);
	}
	minishell->cmd_table[i]->fd_out = minishell->cmd_table[i]->fd_pipe[1];
	minishell->cmd_table[i + 1]->fd_in = minishell->cmd_table[i]->fd_pipe[0];
	ft_printf("in %d\n", minishell->cmd_table[i]->fd_in);
	ft_printf("out %d\n", minishell->cmd_table[i]->fd_out);
	ft_printf("in %d\n", minishell->cmd_table[i + 1]->fd_in);
	ft_printf("out %d\n", minishell->cmd_table[i + 1]->fd_out);
}

void	execute(t_shell *minishell, char *line)
{
	int	i;
	int	nb_commands;

	i = 0;
	nb_commands = minishell->count_pipes;
	while (i < nb_commands)
	{
		// pipe
		if (i < nb_commands - 1)
			ft_set_pipe(minishell, i);
		// open file/redir
		if (minishell->cmd_table[i]->redirs_in
			|| minishell->cmd_table[i]->redirs_out)
		{
			exec_redirs(minishell->cmd_table[i],
				minishell->cmd_table[i]->redirs_in,
				minishell->cmd_table[i]->redirs_out);
		}
		else
		{
			minishell->cmd_table[i]->fd_in = 0;
			minishell->cmd_table[i]->fd_out = 1;
		}
		if (is_builtin(minishell->cmd_table[i]->cmd_args[0])
			&& nb_commands == 1)
		{
			// printf("is_builtin == 1 || 2\n");
			execute_builtin(minishell->cmd_table[i], minishell->env_lst);
		}
		else if (is_builtin(minishell->cmd_table[i]->cmd_args[0]) == 2)
		{
			// printf("is_builtin == 2\n");
			execute_builtin(minishell->cmd_table[i], minishell->env_lst);
		}
		// fork() only for env, pwd and echo
		// otherwise fork and then check
		else
		{
			// printf("else\n");
			exec_in_child(minishell, i);
		}
		if (minishell->cmd_table[i]->fd_out != 1)
			close(minishell->cmd_table[i]->fd_out);
		i++;
	}
}
