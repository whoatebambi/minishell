/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redistribute_node.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbeaudoi <gbeaudoi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/08 13:40:22 by gbeaudoi          #+#    #+#             */
/*   Updated: 2024/06/10 18:19:32 by gbeaudoi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_redir	*ft_last_stack_redir(t_redir *a)
{
	if (!a)
		return (NULL);
	while (a->next)
		a = a->next;
	return (a);
}

static void	ft_stack_add_to_back_redir(t_redir **a, t_redir *new_node)
{
	t_redir	*last_node;

	if (!a || !new_node)
		return ;
	if (*a)
	{
		last_node = ft_last_stack_redir(*a);
		new_node->previous = last_node;
		last_node->next = new_node;
	}
	else
	{
		new_node->previous = *a;
		*a = new_node;
	}
}

static t_redir	*ft_new_redir(t_node *copy)
{
	t_redir	*node;

	node = (t_redir *)malloc(sizeof(t_redir));
	if (node == NULL)
		return (NULL);
	if (!ft_strcmp(copy->string, ">"))
		node->type = REDIR_OUT;
	else if (!ft_strcmp(copy->string, ">>"))
		node->type = APPEND;
	else if (!ft_strcmp(copy->string, "<"))
		node->type = REDIR_IN;
	else
		node->type = DELIMITER;
	node->redir = ft_strdup(copy->next->string);
	if (node->redir == NULL)
	{
		// reset mini
	}
	node->next = NULL;
	node->previous = NULL;
	return (node);
}

void	ft_init_redir_list(t_redir **redir, t_node *nodes, char *token1,
		char *token2)
{
	int		i;
	t_node	*copy;
	t_redir	*new_node;

	*redir = NULL;
	copy = nodes;
	i = 0;
	while (copy && copy->next)
	{
		if (copy->token == 1 && (!ft_strcmp(copy->string, token1)
				|| !ft_strcmp(copy->string, token2)) && copy->next->redir == 1)
		{
			new_node = ft_new_redir(copy);
			if (new_node == NULL)
			{
				// reset mini
			}
			ft_stack_add_to_back_redir(redir, new_node);
		}
		copy = copy->next;
	}
}
