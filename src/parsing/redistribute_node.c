/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redistribute_node.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbeaudoi <gbeaudoi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/08 13:40:22 by gbeaudoi          #+#    #+#             */
/*   Updated: 2024/11/04 11:51:53 by gbeaudoi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

static void	ft_help_redir(t_redir *node, t_node *copy, t_shell *minishell)
{
	if (copy->next)
	{
		node->redir_name = ft_strdup(copy->next->string);
		if (node->redir_name == NULL)
			exitmsg(minishell, MERROR);
		if (copy->next->quote)
			node->quote = 1;
		else
			node->quote = 0;
	}
	else
		node->redir_name = NULL;
}

static t_redir	*ft_new_redir(t_node *copy, t_shell *minishell)
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
	ft_help_redir(node, copy, minishell);
	node->next = NULL;
	node->previous = NULL;
	return (node);
}

void	ft_init_redir_list_in(t_redir **redir, t_node *nodes,
		t_shell *minishell)
{
	t_node	*copy;
	t_redir	*new_node;

	*redir = NULL;
	copy = nodes;
	while (copy && copy->next)
	{
		if (copy->token == 1 && (!ft_strcmp(copy->string, "<")
				|| !ft_strcmp(copy->string, "<<")) && copy->next->redir == 1)
		{
			new_node = ft_new_redir(copy, minishell);
			if (new_node == NULL)
				exitmsg(minishell, MERROR);
			ft_stack_add_to_back_redir(redir, new_node);
		}
		else
			new_node = NULL;
		copy = copy->next;
	}
}

void	ft_init_redir_list_out(t_redir **redir, t_node *nodes,
		t_shell *minishell)
{
	t_node	*copy;
	t_redir	*new_node;

	*redir = NULL;
	copy = nodes;
	while (copy && copy->next)
	{
		if (copy->token == 1 && (!ft_strcmp(copy->string, ">")
				|| !ft_strcmp(copy->string, ">>")) && copy->next->redir == 1)
		{
			new_node = ft_new_redir(copy, minishell);
			if (new_node == NULL)
				exitmsg(minishell, MERROR);
			ft_stack_add_to_back_redir(redir, new_node);
		}
		else
			new_node = NULL;
		copy = copy->next;
	}
}
