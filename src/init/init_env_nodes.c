/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_env_nodes.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbeaudoi <gbeaudoi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/21 20:27:20 by fcouserg          #+#    #+#             */
/*   Updated: 2024/11/05 11:21:57 by gbeaudoi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	set_shlvl_inception(t_shell *minishell, t_env *node)
{
	int		shlvl;
	char	*byte;

	shlvl = ft_atoi(node->value);
	free(node->value);
	node->value = NULL;
	byte = ft_itoa(shlvl + 1);
	if (!byte)
		exitmsg(minishell, "Malloc error");
	node->value = ft_strdup(byte);
	if(!node->value)
		exitmsg(minishell, "Malloc error");
	free(byte);
	byte = NULL;
	if (!node->value)
		exitmsg(minishell, "Malloc error");
}

void	init_env_node(t_shell *minishell, t_env *node, char *str)
{
	node->value = ft_strdup(str);
	if (!node->value)
	{
		free(node->key);
		node->key = NULL;
		free(node);
		free(str);
		exitmsg(minishell, "Malloc error");
	}
	if (safe_strcmp(node->key, "SHLVL") == 0)
		set_shlvl_inception(minishell, node);
	node->isunset = false;
	node->next = NULL;
	node->oldpwd = false;
}

void	init_env_nodes(t_shell *minishell, t_env *node, char **envp, int i)
{
	t_env	*curr;
	int		j;
	char	*str;

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
		exitmsg(minishell, "Malloc error");
	str = getenv(node->key);
	if (!str)
		exitmsg(minishell, "Malloc error");
	init_env_node(minishell, node, str);
}
