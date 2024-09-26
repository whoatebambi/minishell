/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executing.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/21 20:27:20 by fcouserg          #+#    #+#             */
/*   Updated: 2024/09/25 23:57:41 by codespace        ###   ########.fr       */
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


char	**build_execve_path(t_list *env_lst)
{
	char	**execve_path_table;
	int		i;
	
	i = 0;
	execve_path_table = NULL;
	while (env_lst)
	{
		if (ft_strncmp(((t_env *)(env_lst->content))->key, "PATH", 4) == 0)
		{
			execve_path_table = ft_split(((t_env *)(env_lst->content))->value, ':');
			break;
		}
		env_lst = env_lst->next;
	}
	return (execve_path_table);
}

char	**get_execpath(t_shell *shell)
{
	t_env	*env;
	char	**execve_path;

	env = shell->env;
	execve_path = NULL;
	while (env)
	{
		if (safe_strcmp(env->key, "PATH") == 0)
		{
			execve_path = ft_split(env->value, ':');
			break;
		}
		env = env->next;
	}
	return (execve_path);
}

void	prep_exec(t_shell *minishell, t_fds *fd)
{
	t_cmd	*node;
	t_cmd	*cmd;

	fd->redir[0] = -42;
	node = minishell->cmd;
	cmd = minishell->cmd;
	while (cmd)
	{
		// check_builtins(cmd);
			// j = is_builtin(minishell->cmd_table[i]->cmd_args[0]);
		cmd = cmd->next;
	}
	minishell->tabpath = get_execpath(minishell);
	// handle_withpath(shell, node, -1, 0);
}

void	start_exec(t_shell *minishell)
{
	int		i;
	t_fds	fd;

	i = 0;
	prep_exec(minishell, &fd);
	// while (i < minishell->count_pipes)
	// {
	// 	minishell->child_pids[i] = -2;
	// 	ft_init_fds(&fd);
	// 	if (i < minishell->count_pipes - 1)
	// 	{
	// 		if (pipe(fd.pipes) == -1)
	// 			perror("pipe");
	// 			// ft_printf("ERROR");
	// 	}
	// 	exec_redirs(minishell, &fd, i);
	// 	set_redirs(&fd);
	// 	if (is_builtin(minishell->cmd_table[i]->cmd_args[0]) && minishell->count_pipes == 1)
	// 		execute_builtin(minishell->cmd_table[i], minishell->env_lst, &fd);
	// 	else if (is_builtin(minishell->cmd_table[i]->cmd_args[0]) == 2)
	// 		execute_builtin(minishell->cmd_table[i], minishell->env_lst, &fd);
	// 	else
	// 		exec_in_child(minishell, i, &fd);
	// 	if (i + 1 == minishell->count_pipes)
	// 		close_fds_parent(&fd);
	// 	i++;
	// }
	// ft_wait_all_children(minishell);
	// close_fds(&fd);
}

// echo papa >file1 | cat < file1