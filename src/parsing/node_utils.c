/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   node_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbeaudoi <gbeaudoi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/17 15:56:28 by gbeaudoi          #+#    #+#             */
/*   Updated: 2024/06/17 16:00:46 by gbeaudoi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_node	*ft_last_stack(t_node *a)
{
	if (!a)
		return (NULL);
	while (a->next)
		a = a->next;
	return (a);
}

void	ft_stack_add_to_back(t_node **a, t_node *new_node)
{
	t_node	*last_node;

	if (!a || !new_node)
		return ;
	if (*a)
	{
		last_node = ft_last_stack(*a);
		new_node->previous = last_node;
		last_node->next = new_node;
	}
	else
	{
		new_node->previous = *a;
		*a = new_node;
	}
}

t_node	*ft_new_node(char *word, int flag)
{
	t_node	*node;

	node = (t_node *)malloc(sizeof(t_node));
	if (node == NULL)
		return (NULL);
	node->string = ft_strdup(word);
	if (node->string == NULL)
	{
		// reset mini
	}
	node->token = 0;
	node->redir = 0;
	if(flag)
		node->quote = 1;
	else
		node->quote = 0;
	node->next = NULL;
	node->previous = NULL;
	return (node);
}
