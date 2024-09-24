/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_parsing.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcouserg <fcouserg@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/10 15:59:26 by gbeaudoi          #+#    #+#             */
/*   Updated: 2024/09/24 18:18:57 by fcouserg         ###   ########.fr       */
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
void	ft_free_cmd_table_loop(t_cmd_table **cmd_table, int count_pipes)
{
	int	i;

	if (cmd_table == NULL)
		return ;
	i = 0;
	while (i < count_pipes)
	{
		safe_free(cmd_table[i]->group_command);
		ft_free_node(cmd_table[i]->nodes);
		ft_free_double_char(cmd_table[i]->cmd_args);
		ft_free_redir(cmd_table[i]->redirs_in);
		ft_free_redir(cmd_table[i]->redirs_out);
		free(cmd_table[i]);
		i++;
	}
	free(cmd_table);
	cmd_table = NULL;
}

// typedef struct s_cmd_table
// {
// 	char			*group_command;
// 	t_node			*nodes;
// 	char			**cmd_args;
// 	t_redir			*redirs_in;
// 	t_redir			*redirs_out;
// 	int				is_infile_tmp;
// 	char			*infile_tmp;
// }					t_cmd_table;

void	free_cmd_table(t_cmd_table **cmd_table, int count_pipes)
{
	int	i;

	if (cmd_table == NULL)
		return ;
	i = 0;
	// printf("count_pipes = %d\n", count_pipes);
	while (i < count_pipes && cmd_table[i])
	{
		safe_free(cmd_table[i]->group_command);
		ft_free_node(cmd_table[i]->nodes);
		ft_free_double_char(cmd_table[i]->cmd_args);
		ft_free_redir(cmd_table[i]->redirs_in);
		ft_free_redir(cmd_table[i]->redirs_out);
		safe_free(cmd_table[i]->infile_tmp);
		free(cmd_table[i]);
		// printf("freed cmd_table[%d]\n", i);
		i++;
	}
	free(cmd_table);
	// printf("freed **cmd_table\n");
	cmd_table = NULL;
}

void	ft_check_strdup(char *str, int i, char **dest, int flag)
{
	if (flag == 1)
		*dest = ft_strndup(str, i);
	else 
		*dest = ft_strdup(str);
	if (*dest == NULL)
	{
		// reset
	}
}