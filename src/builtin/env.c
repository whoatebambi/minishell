/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/21 20:27:20 by fcouserg          #+#    #+#             */
/*   Updated: 2024/10/28 15:48:41 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	builtin_env(t_env *env, int fd_out, t_shell *minishell)
{
    char    *key;
    char    *value;

	if (env == NULL)
		return ;
	while (env)
	{
        key = env->key;
        value = env->value;
        safe_write(fd_out, key, "=", value, "\n", NULL);
		env = env->next;
	}
	free_minishell(minishell);
	exit(0);
}
