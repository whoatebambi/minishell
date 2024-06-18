/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   node_init.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbeaudoi <gbeaudoi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/05 14:56:48 by gbeaudoi          #+#    #+#             */
/*   Updated: 2024/06/17 15:58:07 by gbeaudoi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*ft_check_token(t_cmd_table *command_table, int *i, char token)
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

static char	*ft_fill_word(t_cmd_table *command_table, int *i, int n)
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
	int flag;

	*nodes = NULL;
	i = 0;
	while (command_table->group_command[i])
	{
		n = 0;
		word = ft_fill_word(command_table, &i, n);
		flag = ft_rev_neg_words(word);
		new_node = ft_new_node(word, flag);
		if (new_node == NULL)
		{
			// reset mini
		}
		ft_stack_add_to_back(nodes, new_node);
		free(word);
	}
}
