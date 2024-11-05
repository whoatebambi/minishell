/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/21 20:27:20 by fcouserg          #+#    #+#             */
/*   Updated: 2024/11/05 00:25:49 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	create_env_list(char *name, char *value, t_shell *minishell)
{
	t_env	*env_var;
	t_env	*tmp;

	env_var = ft_calloc(sizeof(t_env), 1);
	if (!env_var)
		exitmsg(minishell, "Malloc error");
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
