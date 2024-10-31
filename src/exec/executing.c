/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executing.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcouserg <fcouserg@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/21 20:27:20 by fcouserg          #+#    #+#             */
/*   Updated: 2024/10/31 16:06:50 by fcouserg         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
	if (cmd->builtin == ECHO)
		builtin_echo(cmd->tab, shell, i);
	if (cmd->builtin == ENV)
		builtin_env(shell->env, shell);
    if (cmd->builtin == PWD)
		builtin_pwd(STDOUT_FILENO, shell);
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

void	ft_exec(t_shell *minishell, t_cmd_table *cmd, int i, t_fds *fd)
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
				child_builtin(minishell, cmd, fd, i);
			else
				children(minishell, cmd, fd);
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
		*i = *i + 1;
		return (1);
	}
	else if (ft_strchr(shell->cmd_table[*i]->tab[0], '/') && access(shell->cmd_table[*i]->tab[0], F_OK | X_OK) == 0)
	{
		if (stat(shell->cmd_table[*i]->tab[0], &info) == 0 && S_ISDIR(info.st_mode))
		{
			ft_perror(shell, shell->cmd_table[*i]->tab[0], "Is a directory", NULL);
			// safe_write(2, "minishell: ", shell->cmd_table[*i]->tab[0], ": Is a directory\n", NULL);
			shell->excode = 126;
			shell->cmd_table[*i]->pathnoaccess = true;
		}
		shell->cmd_table[*i]->path = ft_strdup(shell->cmd_table[*i]->tab[0]);
		if (!shell->cmd_table[*i]->path)
			exitmsg(shell, MERROR);
		*i = *i + 1;
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
			if (withpath(shell, &(i)))
				continue;
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
			if (!flag && shell->cmd_table[i]->builtin == NOT)
				set_path_anyway(shell, shell->cmd_table[i]);
		}
		i++;
	}
}

void	check_builtins(t_cmd_table *cmd)
{
	if (safe_strcmp(cmd->tab[0], "echo") == 0)
		cmd->builtin = ECHO;
	if (safe_strcmp(cmd->tab[0], "cd") == 0)
		cmd->builtin = CD;
	if (safe_strcmp(cmd->tab[0], "pwd") == 0)
		cmd->builtin = PWD;
	if (safe_strcmp(cmd->tab[0], "export") == 0)
		cmd->builtin = EXPORT;
	if (safe_strcmp(cmd->tab[0], "unset") == 0)
		cmd->builtin = UNSET;
	if (safe_strcmp(cmd->tab[0], "env") == 0)
		cmd->builtin = ENV;
	if (safe_strcmp(cmd->tab[0], "exit") == 0)
		cmd->builtin = EXIT;
}

void	prep_exec(t_shell *minishell, t_fds *fd)
{
	t_env	*env;
	int i;

	fd->redir[0] = -42;
	minishell->tabpath = NULL;
	i = 0;
	env = minishell->env;
	while (i < minishell->count_pipes)
	{
		check_builtins(minishell->cmd_table[i]);
		i++;
	}
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
		if (!minishell->cmd_table[i]->path && minishell->cmd_table[i]->builtin == NOT)
			close_fds(&fd);
		if (minishell->cmd_table[i]->tab[0])
			ft_exec(minishell, minishell->cmd_table[i], i, &fd);
		if (i + 1 == minishell->count_pipes)
			close_fds_parent(&fd);
		i++;
	}
	ft_wait_all_children(minishell);
	close_fds(&fd);
}
