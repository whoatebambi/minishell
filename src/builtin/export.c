/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcouserg <fcouserg@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/21 20:27:20 by fcouserg          #+#    #+#             */
/*   Updated: 2024/10/31 16:48:57 by fcouserg         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_export(t_env *env)
{
	char	*key;
	char	*value;
	t_env	*tmp;

	if (env == NULL)
		return ;
	tmp = env;
	while (tmp)
	{
		key = tmp->key;
		value = tmp->value;
		safe_write(1, "export ", key, "=", value, "\n", NULL);
		tmp = tmp->next;
	}
}

t_env	*get_env_lst(char *name, t_env *env)
{
	t_env	*to_get;
	t_env	*tmp;

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

int	export_check_syntax(int *i, char *arg, t_shell *minishell)
{
	if (!ft_isalpha(arg[0]))
	{
		minishell->excode = 1;
		return (0);
	}
	while (arg[*i] && arg[*i] != '=')
	{
		if (arg[*i] == '-' || arg[*i] == '+')
		{
			minishell->excode = 1;
			return (0);
		}
		*i = *i + 1;
	}
	if (arg[*i] != '=' || !arg[*i + 1])
		return (0);
	return (1);
}

int	add_env_list(char *arg, t_env *env, t_shell *minishell)
{
	int		i;
	char	*key;
	char	*value;

	i = 1;
	if (export_check_syntax(&i, arg, minishell) == 0)
		return (0);
	key = ft_substr(arg, 0, i);
	value = ft_strdup(arg + i + 1);
	if (get_env_lst(key, minishell->env))
	{
		set_env_value(key, value, env);
		free(key);
		free(value);
	}
	else
		create_env_list(key, value, minishell);
	return (1);
}

void	builtin_export(t_env *env, char **tab, t_shell *minishell)
{
	int	i;

	i = 1;
	if (!tab[i])
		print_export(env);
	while (tab[i])
	{
		if (add_env_list(tab[i], env, minishell) == 0)
		{
			if (minishell->excode == 1)
				safe_write(2, "export: `", tab[i],
					"': not a valid identifier\n", NULL);
		}
		i++;
	}
}
