/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_parsing.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcouserg <fcouserg@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/10 15:59:26 by gbeaudoi          #+#    #+#             */
/*   Updated: 2024/06/11 19:01:47 by fcouserg         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_free_child(pid_t *i)
{
	if (!i)
		return ;
	free(i);
}

// to remove in a bit since i make a new one (similar)
void	ft_free_cmd_table3(t_cmd_table **cmd_table, int count_pipes)
{
	int	i;

	if (cmd_table == NULL)
		return ;
	i = 0;
	printf("count_pipes = %d\n", count_pipes);
	while (i < count_pipes)
	{
		ft_free_line(cmd_table[i]->group_command);
		ft_free_node(cmd_table[i]->nodes);
		ft_free_double_char(cmd_table[i]->cmd_args);
		ft_free_redir(cmd_table[i]->redirs_in);
		ft_free_redir(cmd_table[i]->redirs_out);
		// ft_free_int(cmd_table[i]->fd_in);  // if malloc
		// ft_free_int(cmd_table[i]->fd_out); // if malloc
		free(cmd_table[i]);
		printf("freed cmd_table[%d]\n", i);
		i++;
	}
	free(cmd_table);
	printf("freed **cmd_table\n");
	cmd_table = NULL;
}

// to remove in a bit since i make a new one (similar)
void	ft_free_cmd_table2(t_cmd_table **cmd_table, int count_pipes)
{
	int i;

	if (cmd_table == NULL)
		return ;
	i = 0;
	// printf("count_pipes = %d\n", count_pipes);
	ft_printf("1\n");
	while (i < count_pipes)
	{
		ft_printf("a\n");
		ft_free_line(cmd_table[i]->group_command);
		ft_printf("b\n");
		ft_free_node(cmd_table[i]->nodes);
		ft_printf("c\n");
		ft_free_double_char(cmd_table[i]->cmd_args);
		ft_printf("d\n");
		ft_free_redir(cmd_table[i]->redirs_in);
		ft_printf("e\n");
		ft_free_redir(cmd_table[i]->redirs_out);
		ft_printf("f\n");
		free(cmd_table[i]);
		ft_printf("g\n");
		// printf("freed cmd_table[%d]\n", i);
		i++;
	}
	ft_printf("h\n");
	free(cmd_table);
	ft_printf("i\n");
	// printf("freed **cmd_table\n");
	cmd_table = NULL;
}