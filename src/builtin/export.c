/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/21 20:27:20 by fcouserg          #+#    #+#             */
/*   Updated: 2024/10/29 00:03:37 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

void	builtin_export(t_env *env, char **tab, int fd_out, t_shell *minishell)
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
