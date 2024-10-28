/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/21 20:27:20 by fcouserg          #+#    #+#             */
/*   Updated: 2024/10/28 15:47:45 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_builtin(char *cmd_arg)
{
	if (safe_strcmp(cmd_arg, "pwd") == 0)
		return (2);
	else if (safe_strcmp(cmd_arg, "echo") == 0)
		return (2);
	else if (safe_strcmp(cmd_arg, "env") == 0)
		return (2);
	else if (safe_strcmp(cmd_arg, "export") == 0)
		return (2);
    else if (safe_strcmp(cmd_arg, "cd") == 0)
		return (1);
	else if (safe_strcmp(cmd_arg, "unset") == 0)
		return (1);
	else if (safe_strcmp(cmd_arg, "exit") == 0)
		return (1);
	return (0);
}

void	replace_env_var(char *pwd, char *key, t_env *env)
{
	while (env)
	{
		if (safe_strcmp(env->key, key) == 0)
		{
			free(env->value);
			env->value = ft_strdup(pwd);
		}
		env = env->next;
	}
}

void	pwd(int fd_out, t_shell *minishell)
{
	char	*buffer;

	buffer = NULL;
	buffer = getcwd(buffer, 4096); // what value to choose ? 0?
	if (buffer == NULL)
	{
		minishell->excode = 1;
		exitmsg(minishell, "getcwd");
	}
	safe_write(fd_out, buffer, "\n", NULL);
	free(buffer);
	minishell->tmpexcode = minishell->excode;
	minishell->excode = 0;
	exit(0);
}


void	print_export(t_env *env, int fd_out)
{
    char    *key;
    char    *value;
	t_env	*tmp;

	if (env == NULL)
		return ;
	tmp = env;
	while (tmp)
	{
        key = tmp->key;
        value = tmp->value;
        safe_write(fd_out, "export ", key, "=", value, "\n", NULL);
		tmp = tmp->next;
	}
}

t_env	*get_env_lst(char *name, t_env *env)
{
	t_env		*to_get;
	t_env		*tmp;

	to_get = NULL;
	if (!name)
		return (NULL);
	if (!env)
		to_get = env;
	tmp = env;
	while (tmp)
	{
		if (safe_strcmp(tmp->key, name) == 0)
		{
			to_get = tmp;
			break ;
		}
		tmp = tmp->next;
	}
	return (to_get);
}

void	set_env_value(char *name, char *value, t_env *env)
{
	t_env	*to_set;

	if (!value)
		return ;
	to_set = get_env_lst(name, env);
	if (!to_set)
		return ;
	if (to_set->value)
		free(to_set->value);
	to_set->value = NULL;
	to_set->value = ft_strdup(value);
}

void	create_and_add_env_list(char *name, char *value, t_shell *minishell)
{
	t_env	*env_var;
	t_env	*tmp;

	env_var = ft_calloc(sizeof(t_env), 1);
	//
	env_var->key = name;
	env_var->value = value;
	env_var->next = NULL;
	if (minishell->env == NULL)
		minishell->env = env_var;
	else
	{
		tmp = minishell->env;
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = env_var;
	}
}

int	add_env_list(char *arg, t_env *env, int fd_out, t_shell *minishell)
{
	int		i;
	char	*key;
	char	*value;

	if (!ft_isalpha(arg[0]))
	{
		minishell->excode = 1;
		return (0);
	}
	i = 1;
	while (arg[i] && arg[i] != '=')
	{
		if (arg[i] == '-' || arg[i] == '+')
		{
			minishell->excode = 1;
			return (0);
		}
		i++;
	}
	if (arg[i] != '=' || !arg[i + 1])
	{
		// minishell->excode = 0;
		return (0);
	}
	key = ft_substr(arg, 0, i);
	value = ft_strdup(arg + i + 1);
	if (get_env_lst(key, minishell->env))
	{
		set_env_value(key, value, env);
		free(key);
		free(value);
	}
	else
		create_and_add_env_list(key, value, minishell);
	return (1);
}

void	export(t_env *env, char **tab, int fd_out, t_shell *minishell)
{
	int i;

	i = 1;
	if (!tab[i])
		print_export(env, fd_out);
	while (tab[i])
	{
		if (add_env_list(tab[i], env, fd_out, minishell) == 0)
		{
			if (minishell->excode == 1)
				safe_write(2, "export: `", tab[i],"': not a valid identifier\n", NULL);
		}
		i++;
	}
	// exit(minishell->excode);
}

void	unset(char **tab, t_shell *minishell)
{
	int		i;
	t_env	*env;

	i = 1;
	if (!tab[1])
		return ;
	while (tab[i])
	{
		env = minishell->env;
		while (env)
		{
			if (safe_strcmp(env->key, tab[i]) == 0)
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
		i++;
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