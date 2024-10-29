/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcouserg <fcouserg@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/21 20:27:20 by fcouserg          #+#    #+#             */
/*   Updated: 2024/10/29 16:49:34 by fcouserg         ###   ########.fr       */
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
        safe_write(1, key, "=", value, "\n", NULL);
		env = env->next;
	}
	free_minishell(minishell);
	exit(0);
	// int	i;

	// if (cmd->tab[1])
	// {
	// 	ft_perror(shell, cmd->tab[1], NOSUCH, NULL);
	// 	ft_freeshell(shell);
	// 	exit(127);
	// }
	// i = 0;
	// while (shell->envp[i])
	// 	fd_printf(1, "%s\n", shell->envp[i++]);
	// ft_freeshell(shell);
	// exit(0);
}
