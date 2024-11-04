/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbeaudoi <gbeaudoi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/10 17:25:33 by gbeaudoi          #+#    #+#             */
/*   Updated: 2024/11/04 13:22:38 by gbeaudoi         ###   ########.fr       */
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

// if (!redir_def)
// 	return ;
void	ft_free_redir(t_redir *redir_def)
{
	t_redir	*temp_node;

	while (redir_def)
	{
		temp_node = redir_def->next;
		safe_free(redir_def->redir_name);
		free(redir_def);
		redir_def = temp_node;
	}
	redir_def = NULL;
}
// free(redir_def);

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
