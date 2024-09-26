/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_no_env.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/21 20:27:20 by fcouserg          #+#    #+#             */
/*   Updated: 2024/09/25 22:26:32 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	set_oldpwd(t_shell *shell)
{
	t_env	*curr;
	t_env	*new;

	curr = shell->env;
	while (curr->next)
		curr = curr->next;
	new = ft_calloc(1, sizeof(t_env));
	// if (!new)
	// 	exitmsg(shell, MERROR);
	new->key = ft_strdup("OLDPWD");
	// if (!new->key)
	// 	exitmsg(shell, MERROR);
	new->value = ft_strdup("OLDPWD");
	new->index = 4;
	curr->next = new;
	new->next = NULL;
	new->isunset = true;
	new->oldpwd = true;
}

void	set_lastcmd(t_shell *shell)
{
	t_env	*curr;
	t_env	*new;

	curr = shell->env;
	while (curr->next)
		curr = curr->next;
	new = ft_calloc(1, sizeof(t_env));
	// if (!new)
	// 	exitmsg(shell, MERROR);
	new->key = ft_strdup("_");
	// if (!new->key)
	// 	exitmsg(shell, MERROR);
	new->value = ft_strdup("./minishell");
	// if (!new->value)
	// 	exitmsg(shell, MERROR);
	new->index = 3;
	curr->next = new;
	new->next = NULL;
}

void	set_shlvl(t_shell *shell)
{
	t_env	*curr;
	t_env	*new;

	curr = shell->env;
	while (curr->next)
		curr = curr->next;
	new = ft_calloc(1, sizeof(t_env));
	// if (!new)
	// 	exitmsg(shell, MERROR);
	new->key = ft_strdup("SHLVL");
	// if (!new->key)
	// 	exitmsg(shell, MERROR);
	new->value = ft_strdup("1");
	// if (!new->value)
	// 	exitmsg(shell, MERROR);
	new->index = 2;
	curr->next = new;
	new->next = NULL;
}

void	set_pwd(t_shell *shell)
{
	char	*current_dir_name;
	t_env	*new;

	current_dir_name = getcwd(NULL, 0);
	if (current_dir_name == NULL)
	{
		perror("getcwd");
		exit(EXIT_FAILURE);
	}
	new = ft_calloc(1, sizeof(t_env));
	if (new == NULL)
	{
		free(current_dir_name);
		perror("ft_calloc");
		exit(EXIT_FAILURE);
	}
	new->key = ft_strdup("PWD");
	if (new->key == NULL)
	{
		free(current_dir_name);
		free(new);
		perror("ft_strdup");
		exit(EXIT_FAILURE);
	}
	new->value = ft_strdup(current_dir_name);
	if (new->value == NULL)
	{
		free(current_dir_name);
		free(new->key);
		free(new);
		perror("ft_strdup");
		exit(EXIT_FAILURE);
	}
    free(current_dir_name);
	new->index = 1;
	new->next = NULL;
	shell->env = new;
}

void	ft_no_env(t_shell *minishell)
{
	set_pwd(minishell);
	set_shlvl(minishell);
	set_lastcmd(minishell);
	set_oldpwd(minishell);
}