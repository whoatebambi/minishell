/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbeaudoi <gbeaudoi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/10 17:25:33 by gbeaudoi          #+#    #+#             */
/*   Updated: 2024/06/12 16:14:03 by gbeaudoi         ###   ########.fr       */
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
		ft_free_line(node_def->string);
		free(node_def);
		node_def = temp_node;
	}
	free(node_def);
	node_def = NULL;
}

void	ft_free_redir(t_redir *redir_def)
{
	t_redir	*temp_node;

	if (!redir_def)
		return ;
	while (redir_def)
	{
		temp_node = redir_def->next;
		ft_free_line(redir_def->redir);
		free(redir_def);
		redir_def = temp_node;
	}
	//free(redir_def);
	redir_def = NULL;
}

void	ft_free_line(char *string)
{
	if (!string)// || !string[0])
		return ;
	free(string);
	string = NULL;
}

void	ft_free_double_char(char **tab)
{
	int	i;

	if (!tab)// || !tab[0])
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

// void	ft_free_int(int i)
// {
// 	if (!i)
// 		return ;
// 	free((void *)i);
// }
