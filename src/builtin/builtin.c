/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/21 20:27:20 by fcouserg          #+#    #+#             */
/*   Updated: 2024/10/29 00:33:39 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_builtin(char *cmd_arg)
{
	if (safe_strcmp(cmd_arg, "exit") == 0)
		return (1);
	else if (safe_strcmp(cmd_arg, "cd") == 0)
		return (1);	
	else if (safe_strcmp(cmd_arg, "export") == 0)
		return (1);	
	else if (safe_strcmp(cmd_arg, "unset") == 0)
		return (1);	
	else if (safe_strcmp(cmd_arg, "pwd") == 0)
		return (2);
	else if (safe_strcmp(cmd_arg, "echo") == 0)
		return (2);
	else if (safe_strcmp(cmd_arg, "env") == 0)
		return (2);
	return (0);
}

void	builtin_unset(char **tab, t_shell *minishell, int i)
{
	int		j;
	t_env	*env;

	j = 1;
	// if (cmd->prev || cmd->next || !cmd->tab[1])
	// 	return ;
	if (!tab[1])
		return ;
	while (tab[j])
	{
		env = minishell->env;
		while (env)
		{
			if (safe_strcmp(env->key, tab[j]) == 0)
			{
				if (env->value)
					free(env->value);
				env->value = NULL;
				free(env->key);
				env->key = NULL;
				env->isunset = true;
			}
			env = env->next;
		}
		j++;
	}
	fill_envp(minishell);
}

int	exit_args(char **tab, int *flag)
{
	int	i;

	i = 0;
	if (!ft_isdigit(tab[1][0]) && tab[1][0] != '+' && tab[1][0] != '-')
	{
		safe_write(2, "minishell: exit: ", tab[i],": numeric argument required\n", NULL);
		(*flag) = 1;
		return (2);
	}
	while (tab[1][++i])
	{
		if (!ft_isdigit(tab[1][i]) && !(*flag))
		{
			safe_write(2, "minishell: exit: ", tab[i],": numeric argument required\n", NULL);
			(*flag) = 1;
			return (2);
		}
	}
	return (ft_atoi(tab[1]) % 256);
}

void	ft_exit(char **tab, t_shell *minishell, t_fds *fd)
{
	int	ext;
	int	flag;

	flag = 0;
	if (tab[1])
		minishell->tmpexcode = exit_args(tab, &flag);
	if (tab[1] && tab[2] && !flag)
	{
		safe_write(2, "minishell: `", tab[0],"': too many arguments\n", NULL);
		safe_write(1, "exit\n", NULL);
		minishell->excode = 1;
		minishell->tmpexcode = 1;
		return ;
	}
	// if (cmd->next || cmd->prev)
	// 	return ;
	ext = minishell->tmpexcode;
	free_minishell(minishell);
	close_fds(fd);
	safe_write(1, "exit\n", NULL);
	// printf("ft_exit excode = %d\n", minishell->excode);
	exit(ext);
}
