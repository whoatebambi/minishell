/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcouserg <fcouserg@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/21 20:27:20 by fcouserg          #+#    #+#             */
/*   Updated: 2024/10/31 16:37:52 by fcouserg         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	builtin_unset(char **tab, t_shell *minishell)
{
	int		j;
	t_env	*env;

	j = 1;
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

// if (cmd->prev || cmd->next || !cmd->tab[1])
// 	return ;