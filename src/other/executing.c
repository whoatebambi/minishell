/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executing.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/21 20:27:20 by fcouserg          #+#    #+#             */
/*   Updated: 2024/10/28 15:48:15 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	execute_builtin(t_cmd_table *cmd_table, t_shell *minishell, t_fds *fd, int i)
{
	if (safe_strcmp(cmd_table->tab[0], "pwd") == 0)
		pwd(STDOUT_FILENO, minishell);
    else if (safe_strcmp(cmd_table->tab[0], "cd") == 0)
		builtin_cd(cmd_table->tab, minishell->env, minishell);
	else if (safe_strcmp(cmd_table->tab[0], "echo") == 0)
		builtin_echo(cmd_table->tab, STDOUT_FILENO, minishell, i);
	else if (safe_strcmp(cmd_table->tab[0], "env") == 0)
		builtin_env(minishell->env, STDOUT_FILENO, minishell);
	else if (safe_strcmp(cmd_table->tab[0], "export") == 0)
		export(minishell->env, cmd_table->tab, STDOUT_FILENO, minishell);
	else if (safe_strcmp(cmd_table->tab[0], "unset") == 0)
		unset(cmd_table->tab, minishell);
	else if (safe_strcmp(cmd_table->tab[0], "exit") == 0)
		ft_exit(cmd_table->tab, minishell, fd);
	return (1);
}

void	child_builtin(t_shell *shell, t_cmd_table *cmd, t_fds *fd, int i)
{
	if (fd->savedpipe != -42)
		close(fd->savedpipe);
	if (fd->pipes[0] != -42)
		close(fd->pipes[0]);
	if (fd->redir[0] != -42)
		if (dup2(fd->redir[0], STDIN_FILENO) == -1)
			(close_fds(fd), exitmsg(shell, NULL));
	if (fd->redir[1] != -42)
		if (dup2(fd->redir[1], STDOUT_FILENO) == -1)
			(close_fds(fd), exitmsg(shell, NULL));
	close_fds(fd);
	execute_builtin(cmd, shell, fd, i);
	// exec_builtin(shell, cmd);
}

void	children(t_shell *shell, t_cmd_table *cmd, t_fds *fd)
{
	if (fd->pipes[0] != -42)
		close(fd->pipes[0]);
	if (fd->savedpipe != -42)
		close(fd->savedpipe);
	if (fd->redir[0] != -42)
		if (dup2(fd->redir[0], STDIN_FILENO) == -1)
			(close_fds(fd), exitmsg(shell, NULL));
	if (fd->redir[1] != -42)
		if (dup2(fd->redir[1], STDOUT_FILENO) == -1)
			(close_fds(fd), exitmsg(shell, NULL));
	if (cmd->pathnoaccess)
	{
		if (!cmd->path)
			ft_perror(shell, cmd->tab[0], strerror(errno), NULL);
		close_fds(fd);
		exitmsg(shell, NULL);
	}
	else if (!cmd->path)
	{
		close_fds(fd);
		exitmsg(shell, NULL);
	}
	close_fds(fd);
	if (execve(cmd->path, cmd->tab, shell->envp) == -1)
		(ft_perror(shell, cmd->path, CMDFAIL, NULL), exitmsg(shell, NULL));
}

void	ft_exec(t_shell *minishell, int i, t_fds *fd)
{
	if (is_builtin(minishell->cmd_table[i]->tab[0]) == 1)
		execute_builtin(minishell->cmd_table[i], minishell, fd, i);
	else
	{
		minishell->child_pids[i] = fork();
		if (minishell->child_pids[i] == -1)
			exitmsg(minishell, "fork");
		if (minishell->child_pids[i] == 0)
		{
			if (is_builtin(minishell->cmd_table[i]->tab[0]) == 2)
				child_builtin(minishell, minishell->cmd_table[i], fd, i);
			else
				children(minishell, minishell->cmd_table[i], fd);
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
static void	set_path_anyway(t_shell *shell, t_cmd_table *c)
{
	if (c->path)
		(free(c->path), c->path = NULL);
	shell->excode = 127;
	c->path = ft_strdup(c->tab[0]);
	if (!c->path)
		exitmsg(shell, MERROR);
}

static void	set_cmd_path(t_shell *shell, t_cmd_table *c, char *tmp, int *flag)
{
	free(c->tab[0]);
	c->tab[0] = ft_strdup(tmp);
	if (!c->tab[0])
		(free(tmp), c->tab[0] = NULL, exitmsg(shell, MERROR));
	c->path = ft_strdup(tmp);
	if (!c->path)
		(free(tmp), exitmsg(shell, MERROR));
	(*flag) = 1;
}

int	withpath(t_shell *shell, int *i)
{
	struct stat	info;

	if (ft_strchr(shell->cmd_table[*i]->tab[0], '/') && access(shell->cmd_table[*i]->tab[0], F_OK | X_OK) != 0)
	{
		shell->excode = 127;
		shell->cmd_table[*i]->pathnoaccess = true;
		*i++;
		return (1);
	}
	else if ((ft_strchr(shell->cmd_table[*i]->tab[0], '/')) && access(shell->cmd_table[*i]->tab[0], F_OK | X_OK) == 0)
	{
		if (stat(shell->cmd_table[*i]->tab[0], &info) == 0 && S_ISDIR(info.st_mode))
		{
			// ft_perror(shell, cmd_table[i]->tab[0], "Is a directory", NULL);
			safe_write(2, "minishell: ", shell->cmd_table[*i]->tab[0], ": Is a directory\n", NULL);
			shell->excode = 126;
			shell->cmd_table[*i]->pathnoaccess = true;
		}
		shell->cmd_table[*i]->path = ft_strdup(shell->cmd_table[*i]->tab[0]);
		if (!shell->cmd_table[*i]->path)
			exitmsg(shell, MERROR);
		*i++;
		return (1);
	}
	return (0);
}

void	handle_withpath(t_shell *shell, int flag)
{
	char	*tmp1;
	char	*tmp2;
	int 	i;
	int 	j;

	i = 0;
	while (i < shell->count_pipes)
	{
		j = -1;
		if (shell->cmd_table[i]->tab[0])
		{
			if (withpath(shell, &i))
				continue ;
			flag = 0;
			while (shell->tabpath && shell->tabpath[++j])
			{
				tmp1 = ft_strjoin_no_free("/", shell->cmd_table[i]->tab[0]);
				tmp2 = ft_strjoin_no_free(shell->tabpath[j], tmp1);
				free(tmp1);
				tmp1 = NULL;
				if (!tmp2)
					exitmsg(shell, MERROR);
				if (access(tmp2, X_OK) == 0)
					set_cmd_path(shell, shell->cmd_table[i], tmp2, &flag);
				(free(tmp2), tmp2 = NULL);
			}
			if (!flag && is_builtin(shell->cmd_table[i]->tab[0]) == 0)
				set_path_anyway(shell, shell->cmd_table[i]);
		}
		i++;
	}
}

void	prep_exec(t_shell *minishell, t_fds *fd)
{
	t_env	*env;
	char	*execve_path;

	// printf("prep_exec\n");
	fd->redir[0] = -42;
	minishell->tabpath = NULL;
	env = minishell->env;
	while (env)
	{
		if (safe_strcmp(env->key, "PATH") == 0)
		{
			minishell->tabpath = ft_split(env->value, ':');
			break;
		}
		env = env->next;
	}
	handle_withpath(minishell, 0);
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
		ft_pipes(minishell, &fd, i);
		handle_redirs(minishell, &fd, i);
		if (minishell->excode == 130)
			break ;
		set_redirs(&fd);
		if (!minishell->cmd_table[i]->path && is_builtin(minishell->cmd_table[i]->tab[0]) == 0)
			close_fds(&fd);
		if (minishell->cmd_table[i]->tab[0])
			ft_exec(minishell, i, &fd);
		if (i + 1 == minishell->count_pipes)
			close_fds_parent(&fd);
		i++;
	}
	ft_wait_all_children(minishell);
	close_fds(&fd);
}
