/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_init.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/05 16:11:15 by gbeaudoi          #+#    #+#             */
/*   Updated: 2024/10/23 15:58:43 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

void	ft_fill_arg(t_node *nodes, t_cmd_table **command_table)
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
			{
				// reset;
			}
			i++;
		}
		copy = copy->next;
	}
	(*command_table)->tab[i] = NULL;
	i = 0;
}

void	ft_redistribute_node(t_cmd_table **command_table, t_node *nodes)
{
	int	count_arg;

	count_arg = ft_count_arg(nodes);
	(*command_table)->tab = malloc(sizeof(char *) * (count_arg + 1));
	if ((*command_table)->tab == NULL)
	{
		// reset;
	}
	ft_fill_arg(nodes, command_table);
	ft_init_redir_list(&((*command_table)->redirs_in), nodes, "<", "<<");
	ft_init_redir_list(&((*command_table)->redirs_out), nodes, ">", ">>");
}

void	ft_check_syntax(t_node *nodes)
{
	t_node	*copy;

	copy = nodes;
	while (copy)
	{
		if (copy->token == 1 && copy->next && copy->next->token == 1)
		{
			perror("Syntax ERROR");
			exit(3);
		}
		if (copy->token == 1 && !copy->next)
		{
			perror("Syntax ERROR");
			exit(3);
		}
		copy = copy->next;
	}
}

void	ft_tokenize_list(t_node *nodes, t_shell *minishell)
{
	t_node	*copy;

	if (!nodes || !nodes->string)
		exitmsg(minishell, MERROR);
	copy = nodes;
	while (copy)
	{
		if (!ft_strcmp((copy)->string, ">") || !ft_strcmp((copy)->string, ">>")
			|| !ft_strcmp((copy)->string, "<") || !ft_strcmp((copy)->string,
				"<<"))
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
