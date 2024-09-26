/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   node_init.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/05 14:56:48 by gbeaudoi          #+#    #+#             */
/*   Updated: 2024/09/26 17:10:49 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*ft_check_token(t_cmd_table *command_table, int *i, char token, t_shell *minishell)
{
	char	*dup;

	if (command_table->group_command[*i + 1] == token)
	{
		dup = ft_strndup(command_table->group_command + *i, 2);
		if (dup == NULL)
			exitmsg(minishell, MERROR);
		*i += 2;
	}
	else
	{
		dup = ft_strndup(command_table->group_command + *i, 1);
		if (dup == NULL)
			exitmsg(minishell, MERROR);
		*i += 1;
	}
	return (dup);
}

static char	*ft_fill_word(t_cmd_table *command_table, int *i, int n, t_shell *minishell)
{
	char	*dup;

	while (command_table->group_command[*i] == ' ')
		*i += 1;
	if (command_table->group_command[*i] == '>')
		dup = ft_check_token(command_table, i, '>', minishell);
	else if (command_table->group_command[*i] == '<')
		dup = ft_check_token(command_table, i, '<', minishell);
	else
	{
		while (command_table->group_command[*i + n]
			&& (command_table->group_command[*i + n] != ' '
				&& command_table->group_command[*i + n] != '>'
				&& command_table->group_command[*i + n] != '<'))
			n++;
		dup = ft_strndup(command_table->group_command + *i, n);
		if (dup == NULL)
			exitmsg(minishell, MERROR);
		*i += n;
	}
	while (command_table->group_command[*i] == ' ')
		*i += 1;
	return (dup);
}

void	ft_init_node_list(t_node **nodes, t_cmd_table *command_table, t_shell *minishell)
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
		word = ft_fill_word(command_table, &i, n, minishell);
		flag = ft_rev_neg_words(word);
		new_node = ft_new_node(word, flag);
		if (new_node == NULL)
		{
			free(word);
			exitmsg(minishell, MERROR);
		}
		ft_stack_add_to_back(nodes, new_node);
		free(word);
	}
}
