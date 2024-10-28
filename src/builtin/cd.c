/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/21 20:27:20 by fcouserg          #+#    #+#             */
/*   Updated: 2024/10/28 14:56:46 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void    builtin_cd(char **tab, t_env *env, t_shell *minishell)
{
	char	old_pwd[1024];
    char	new_pwd[1024];

	if (tab[2])
	{
		safe_write(2, "cd: too many arguments\n", NULL);
		minishell->excode = 1;
		exitmsg(minishell, NULL);
	}
	if (getcwd(old_pwd, sizeof(old_pwd)) == NULL)
	{
		minishell->excode = 1;
		exitmsg(minishell, "getcwd");
	}
    if (tab[1] == NULL)
		minishell->excode = chdir(getenv("HOME"));
	else if (safe_strcmp(tab[1], "-") == 0)
		minishell->excode = chdir(getenv("OLDPWD"));
	else
		minishell->excode = chdir(tab[1]);
    if (minishell->excode == -1)
	{
		minishell->excode = 1;
		safe_write(2, "cd: ", tab[1],": No such file or directory\n", NULL);
		exitmsg(minishell, NULL);
	}
	if (getcwd(new_pwd, sizeof(new_pwd)) == NULL)
	{
		minishell->excode = 1;
		exitmsg(minishell, "getcwd");
	}
	replace_env_var(old_pwd, "OLDPWD", env);
	replace_env_var(new_pwd, "PWD", env);
	if (tab[1] && safe_strcmp(tab[1], "-") == 0)
		pwd(1, minishell);
}

// set home....