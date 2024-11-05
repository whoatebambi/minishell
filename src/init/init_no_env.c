/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_no_env.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/21 20:27:20 by fcouserg          #+#    #+#             */
/*   Updated: 2024/11/05 02:15:44 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	set_oldpwd(t_shell *minishell)
{
	t_env	*curr;
	t_env	*new;

	curr = minishell->env;
	while (curr->next)
		curr = curr->next;
	new = ft_calloc(1, sizeof(t_env));
	if (!new)
		exitmsg(minishell, "Malloc error");
	new->key = ft_strdup("OLDPWD");
	if (!new->key)
		exitmsg(minishell, "Malloc error");
	new->value = ft_strdup("OLDPWD");
	new->index = 4;
	curr->next = new;
	new->next = NULL;
	new->isunset = true;
	new->oldpwd = true;
}

void	set_last_command(t_shell *minishell)
{
	t_env	*curr;
	t_env	*new;

	curr = minishell->env;
	while (curr->next)
		curr = curr->next;
	new = ft_calloc(1, sizeof(t_env));
	if (!new)
		exitmsg(minishell, "Malloc error");
	new->key = ft_strdup("_");
	if (!new->key)
		exitmsg(minishell, "Malloc error");
	new->value = ft_strdup("./minishell");
	if (!new->value)
		exitmsg(minishell, "Malloc error");
	new->index = 3;
	curr->next = new;
	new->next = NULL;
}

void	set_shlvl(t_shell *minishell)
{
	t_env	*curr;
	t_env	*new;

	curr = minishell->env;
	while (curr->next)
		curr = curr->next;
	new = ft_calloc(1, sizeof(t_env));
	if (!new)
		exitmsg(minishell, "Malloc error");
	new->key = ft_strdup("SHLVL");
	if (!new->key)
		exitmsg(minishell, "Malloc error");
	new->value = ft_strdup("1");
	if (!new->value)
		exitmsg(minishell, "Malloc error");
	new->index = 2;
	curr->next = new;
	new->next = NULL;
}

void	init_pwd(t_shell *minishell)
{
	char	*current_dir_name;
	t_env	*new;

	current_dir_name = getcwd(NULL, 0);
	if (!current_dir_name)
		exitmsg(minishell, "Malloc error");
	new = ft_calloc(1, sizeof(t_env));
	if (!new)
	{
		free(current_dir_name);
		exitmsg(minishell, "Malloc error");
	}
	new->value = ft_strdup(current_dir_name);
	if (!new->value)
		free_specific_env(new, current_dir_name, minishell);
	free(current_dir_name);
	new->key = ft_strdup("PWD");
	if (!new->key)
		free_specific_env(new, new->value, minishell);
	new->index = 1;
	new->next = NULL;
	minishell->env = new;
}

void	set_no_env(t_shell *minishell)
{
	init_pwd(minishell);
	set_shlvl(minishell);
	set_last_command(minishell);
	set_oldpwd(minishell);
}
