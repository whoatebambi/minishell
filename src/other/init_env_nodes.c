/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_env_nodes.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/21 20:27:20 by fcouserg          #+#    #+#             */
/*   Updated: 2024/10/16 12:00:37 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	set_shlvl_inception(t_shell *minishell, t_env *node) // check if used
{
	int		shlvl;
	char	*byte;

	
	shlvl = ft_atoi(node->value);
	free(node->value);
	node->value = NULL;
	byte = ft_itoa(shlvl + 1);
	if (!byte)
		exitmsg(minishell, MERROR);
	node->value = ft_strdup(byte);
	free(byte);
	if (!node->value)
		exitmsg(minishell, MERROR);
}

void	init_env_nodes(t_shell *minishell, t_env *node, char **envp, int i)
{
	t_env	*curr;
	int j;
	char *str;

	j = 0;	
	curr = minishell->env;
	if (curr == NULL)
		minishell->env = node;
	else
	{
		while (curr->next)
			curr = curr->next;
		curr->next = node;
	}
	node->index = i;
	while (envp[i][j] != '=' && envp[i][j])
		j++;
	node->key = ft_substr(envp[i], 0, j);
	if (!node->key)
		exitmsg(minishell, MERROR);
	str = getenv(node->key);
	if (!str)
		exitmsg(minishell, MERROR);
	node->value = ft_strdup(str);
	if (!node->value)
		exitmsg(minishell, MERROR);
	if (safe_strcmp(node->key, "SHLVL") == 0)
		set_shlvl_inception(minishell, node);
	node->isunset = false;
	node->next = NULL;
	node->oldpwd = false;
}
