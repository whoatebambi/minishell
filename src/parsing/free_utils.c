/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcouserg <fcouserg@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/10 17:25:33 by gbeaudoi          #+#    #+#             */
/*   Updated: 2024/10/31 17:44:37 by fcouserg         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_free_node(t_node *node_def)
{
	t_node	*temp_node;

	if (!node_def)
		return ;
	while (node_def)
	{
		temp_node = node_def->next;
		safe_free(node_def->string);
		free(node_def);
		node_def = temp_node;
	}
	free(node_def);
	node_def = NULL;
}

void	ft_free_redir(t_redir *redir_def)
{
	t_redir	*temp_node;

	// if (!redir_def)
	// 	return ;
	while (redir_def)
	{
		temp_node = redir_def->next;
		safe_free(redir_def->redir_name);
		free(redir_def);
		redir_def = temp_node;
	}
	//free(redir_def);
	redir_def = NULL;
}

void	safe_free(char *string)
{
	if (string)
	{
		free(string);
		string = NULL;
	}
}

void	ft_free_double_char(char **tab)
{
	int	i;

	if (!tab)
		return ;
	i = 0;
	while (tab[i])
	{
		free(tab[i]);
		i++;
	}
	free(tab);
	tab = NULL;
}
