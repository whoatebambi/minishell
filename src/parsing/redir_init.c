/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_init.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/05 16:11:15 by gbeaudoi          #+#    #+#             */
/*   Updated: 2024/11/05 00:25:49 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_redir	*ft_last_stack_redir(t_redir *a)
{
	if (!a)
		return (NULL);
	while (a->next)
		a = a->next;
	return (a);
}

int	ft_count_arg(t_node *nodes)
{
	t_node	*copy;
	int		count;

	copy = nodes;
	count = 0;
	while (copy)
	{
		if (copy->token == 0 && copy->redir == 0)
			count++;
		copy = copy->next;
	}
	return (count);
}

void	ft_fill_arg(t_node *nodes, t_cmd_table **command_table,
		t_shell *minishell)
{
	t_node	*copy;
	int		i;

	copy = nodes;
	i = 0;
	while (copy)
	{
		if (copy->token == 0 && copy->redir == 0)
		{
			(*command_table)->tab[i] = ft_strdup(copy->string);
			if ((*command_table)->tab[i] == NULL)
				exitmsg(minishell, "Malloc error");
			i++;
		}
		copy = copy->next;
	}
	(*command_table)->tab[i] = NULL;
	i = 0;
}

void	ft_redistribute_node(t_cmd_table **command_table, t_node *nodes,
		t_shell *minishell)
{
	int	count_arg;

	count_arg = ft_count_arg(nodes);
	(*command_table)->tab = malloc(sizeof(char *) * (count_arg + 1));
	if ((*command_table)->tab == NULL)
		exitmsg(minishell, "Malloc error");
	ft_fill_arg(nodes, command_table, minishell);
	ft_init_redir_list_in(&((*command_table)->redirs_in), nodes, minishell);
	ft_init_redir_list_out(&((*command_table)->redirs_out), nodes, minishell);
}

void	ft_tokenize_list(t_node *nodes, t_shell *minishell)
{
	t_node	*copy;

	if (!nodes || !nodes->string)
		exitmsg(minishell, "Malloc error");
	copy = nodes;
	while (copy)
	{
		if ((!ft_strcmp((copy)->string, ">") || !ft_strcmp((copy)->string, ">>")
				|| !ft_strcmp((copy)->string, "<") || !ft_strcmp((copy)->string,
					"<<")) && (copy)->quote != 1)
			(copy)->token = 1;
		else
			(copy)->token = 0;
		if ((copy)->token == 0)
		{
			if ((copy)->previous && (copy)->previous->token == 1)
				(copy)->redir = 1;
			else
				(copy)->redir = 0;
		}
		copy = (copy)->next;
	}
}
