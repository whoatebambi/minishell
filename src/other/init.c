/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/21 20:27:20 by fcouserg          #+#    #+#             */
/*   Updated: 2024/10/06 21:38:22 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	init_argc(int argc, char **argv, int fd)
{
	fd = 0;
	if (argc > 2) // usage is either "./minishell" or "./minishell <file>"
	{
		errno = E2BIG; // the int errno is set to the macro "E2BIG"
		perror("Error");
		exit(E2BIG); // "E2BIG" macro from <errno.h> expands to "Argument list too long."
	}
	if (argc == 2) // opens the file (we are in non-interactive mode)
		fd = open(argv[1], O_RDONLY);
	if (fd == -1)
	{
		// no need to set errno to a macro here, the system call open() automatically did it
		perror("Error");
		exit(EXIT_FAILURE);
	}
    return (fd);
}

void    fill_env(t_shell *minishell, char **envp)
{
    t_env	*new_node;
	int		i;

	i = 0;
	if (!envp || !envp[i])
	{
		ft_no_env(minishell);
		return ;
	}
	while (envp && envp[i])
	{
		new_node = malloc(sizeof(t_env));
		if (!new_node)
			exitmsg(minishell, MERROR);
		init_env_nodes(minishell, new_node, envp, i);
		i++;
	}
}


int	env_size(t_shell *minishell)
{
	t_env	*node;
	int		len;

	len = 0;
	node = minishell->env;
	while (node)
	{
		if (node->key)
			len++;
		node = node->next;
	}
	return (len);
}

void	fill_envp(t_shell *minishell)
{
	int		len;
	int		i;
	t_env	*curr;

	i = 0;
	len = env_size(minishell);
	curr = minishell->env;
	minishell->envp = ft_calloc(len + 1, sizeof(char *));
	if (!minishell->envp)
		exitmsg(minishell, MERROR);
	while (curr)
	{
		if (curr->key && !curr->isunset) // isunset????
		{
			minishell->envp[i] = safe_join_envp(curr->key, "=", curr->value);
			if (minishell->envp[i] == NULL)
				exitmsg(minishell, MERROR);
			i++;
		}
		curr = curr->next;
	}
}

char	*getpath(t_shell *minishell, char *key)
{
	t_env	*env;

	env = minishell->env;
	while (env)
	{
		if (safe_strcmp(env->key, key) == 0) // ou l'inverse ?
			break ;
		env = env->next;
	}
	if (!env)
		return (NULL);
	return (env->value);
}

void	fill_path(t_shell *minishell)
{
	minishell->path = ft_calloc(1, sizeof(t_path));
	if (!minishell->path)
		exitmsg(minishell, MERROR);
	char *str;
	str = getpath(minishell, "PWD");
	if (str)
	{
		minishell->path->pwd = ft_strdup(str);
		if (!minishell->path->pwd)
			exitmsg(minishell, MERROR);
	}
	str = getpath(minishell, "OLDPWD");
	if (str)
	{
		minishell->path->oldpwd = ft_strdup(str);
		if (!minishell->path->oldpwd)
			exitmsg(minishell, MERROR);
	}
}


void	init_minishell(t_shell	*minishell, char **envp, int argc)
{
	if (argc == 1)
		minishell->mode = INTERACTIVE;
	else
		minishell->mode = NON_INTERACTIVE;
	minishell->env = NULL;
	fill_env(minishell, envp);
	minishell->envp = NULL;
	fill_envp(minishell);
	minishell->path = NULL;
	fill_path(minishell);
	minishell->lex = NULL;
	minishell->cmd_table = NULL;
    minishell->child_pids = NULL;
	minishell->count_pipes = 0;
	minishell->tmpexcode = 0;
	minishell->excode = 0;
    minishell->line = NULL;
    minishell->clean_line = NULL;
	minishell->execve_path = NULL;
	minishell->inflagerr = 0;
	minishell->outflagerr = 0;
	minishell->tabpath = NULL;
	minishell->inp = NULL;
	minishell->newinp = NULL;
	minishell->finalinp = NULL;
}

int	init_fd(int argc, char **argv, int fd)
{
	fd = 0;
	if (argc == 2)
		fd = open(argv[1], O_RDONLY);
	if (fd == -1) // exitmsg(minishell, MERROR);
	{
		perror("Error");
		exit(EXIT_FAILURE);
	}
    return (fd);
}
