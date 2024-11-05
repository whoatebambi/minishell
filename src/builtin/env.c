/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/21 20:27:20 by fcouserg          #+#    #+#             */
/*   Updated: 2024/11/05 00:04:42 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	builtin_env(t_env *env, t_shell *minishell)
{
	char	*key;
	char	*value;

	if (env == NULL)
		return ;
	while (env)
	{
		key = env->key;
		value = env->value;
		safe_write(1, key, "=", value, "\n", NULL);
		env = env->next;
	}
	free_minishell(minishell);
	exit(0);
}

// int	i;
// if (cmd->tab[1])
// {
// 	ft_perror(minishell, cmd->tab[1], NOSUCH, NULL);
// 	ft_freeshell(minishell);
// 	exit(127);
// }
// i = 0;
// while (minishell->envp[i])
// 	fd_printf(1, "%s\n", minishell->envp[i++]);
// ft_freeshell(minishell);
// exit(0);