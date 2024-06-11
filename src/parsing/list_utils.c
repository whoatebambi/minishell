/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbeaudoi <gbeaudoi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/05 14:56:48 by gbeaudoi          #+#    #+#             */
/*   Updated: 2024/06/10 18:12:43 by gbeaudoi         ###   ########.fr       */
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

static void	ft_stack_add_to_back(t_node **a, t_node *new_node)
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

static t_node	*ft_new_node(char *word)
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
	node->next = NULL;
	node->previous = NULL;
	return (node);
}

char	*ft_check_token(t_cmd_table *command_table, int *i, char token)
{
	char	*dup;

	if (command_table->group_command[*i + 1] == token)
	{
		dup = ft_strndup(command_table->group_command + *i, 2);
		if (dup == NULL)
		{
			// reset mini
		}
		*i += 2;
	}
	else
	{
		dup = ft_strndup(command_table->group_command + *i, 1);
		if (dup == NULL)
		{
			// reset mini
		}
		*i += 1;
	}
	return (dup);
}

char	*ft_fill_word(t_cmd_table *command_table, int *i, int n)
{
	char	*dup;

	while (command_table->group_command[*i] == ' ')
		*i += 1;
	if (command_table->group_command[*i] == '>')
		dup = ft_check_token(command_table, i, '>');
	else if (command_table->group_command[*i] == '<')
		dup = ft_check_token(command_table, i, '<');
	else
	{
		while (command_table->group_command[*i + n]
			&& (command_table->group_command[*i + n] != ' '
				&& command_table->group_command[*i + n] != '>'
				&& command_table->group_command[*i + n] != '<'))
			n++;
		dup = ft_strndup(command_table->group_command + *i, n);
		if (dup == NULL)
		{
			// reset mini
		}
		*i += n;
	}
	while (command_table->group_command[*i] == ' ')
		*i += 1;
	return (dup);
}

void	ft_init_node_list(t_node **nodes, t_cmd_table *command_table)
{
	int		i;
	int		n;
	t_node	*new_node;
	char	*word;

	*nodes = NULL;
	i = 0;
	while (command_table->group_command[i])
	{
		n = 0;
		word = ft_fill_word(command_table, &i, n);
		ft_rev_neg_words(word);
		new_node = ft_new_node(word);
		if (new_node == NULL)
		{
			// reset mini
		}
		ft_stack_add_to_back(nodes, new_node);
		free(word);
	}
}
