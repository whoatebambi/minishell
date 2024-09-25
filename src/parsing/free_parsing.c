/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_parsing.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcouserg <fcouserg@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/10 15:59:26 by gbeaudoi          #+#    #+#             */
/*   Updated: 2024/09/25 16:55:58 by fcouserg         ###   ########.fr       */
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

void	free_cmd_table(t_shell *minishell)
{
	int i = 0;
	
	if (minishell->cmd_table == NULL)
		return ;
	while (i < minishell->count_pipes)
	{
		if (minishell->cmd_table[i] != NULL)
		{
			safe_free(minishell->cmd_table[i]->group_command);
			ft_free_node(minishell->cmd_table[i]->nodes);
			ft_free_double_char(minishell->cmd_table[i]->cmd_args);
			ft_free_redir(minishell->cmd_table[i]->redirs_in);
			ft_free_redir(minishell->cmd_table[i]->redirs_out);
			safe_free(minishell->cmd_table[i]->infile_tmp);
            free(minishell->cmd_table[i]);
            minishell->cmd_table[i] = NULL;
        }
		i++;
	}
	free(minishell->cmd_table);
	minishell->cmd_table = NULL;
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