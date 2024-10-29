/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/21 20:27:20 by fcouserg          #+#    #+#             */
/*   Updated: 2024/10/29 00:00:37 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	builtin_pwd(int fd_out, t_shell *minishell)
{
	char	*pwd;

	// pwd = getcwd(pwd, 4096); // what value to choose ? 0?
    pwd = getcwd(NULL, 0);
	if (pwd == NULL)
	{
		minishell->excode = 1;
		exitmsg(minishell, "getcwd");
	}
	safe_write(fd_out, pwd, "\n", NULL);
	free(pwd);
	free_minishell(minishell);
	exit(0);
}
