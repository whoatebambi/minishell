/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parseur.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcouserg <fcouserg@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/20 15:42:08 by gbeaudoi          #+#    #+#             */
/*   Updated: 2024/09/24 18:05:48 by fcouserg         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	ft_find_pipes(char **pipes)
{
	int	i;

	i = 0;
	while (pipes[i])
		i++;
	return (i);
}

void	ft_init_command_group(t_cmd_table **command_table, char **pipes,
		t_shell *minishell)
{
	int	i;

	i = 0;
	while (pipes[i])
	{
		command_table[i] = ft_calloc(1, sizeof(t_cmd_table));
		// if (command_table[i] == NULL)
		// {
		// 	return ;
		// 	// reset mini
		// }
		command_table[i]->group_command = ft_strdup(pipes[i]);
		// if (command_table[i]->group_command == NULL)
		// {
		// 	return ;
		// 	// reset mini
		// }
		
		ft_init_node_list(&((command_table[i])->nodes), command_table[i]);
		ft_tokenize_list(command_table[i]->nodes);
		ft_check_syntax(command_table[i]->nodes);
		ft_redistribute_node(&command_table[i], command_table[i]->nodes);
		command_table[i]->is_infile_tmp = 0;
		command_table[i]->infile_tmp = NULL;
		i++;
	}
}

int	ft_check_pipes(t_shell *minishell)
{
	int	i;

	i = 0;
	while (minishell->clean_line[i])
	{
		if (minishell->clean_line[i] == '|')
		{
			i++;
			while (minishell->clean_line[i] == ' ')
				i++;
			if (minishell->clean_line[i] == '|' || !minishell->clean_line[i])
				return (0);
		}
		i++;
	}
	return (1);
}

static void	ft_split_pipe(t_shell *minishell)
{
	char	**pipes;
	int		i;

	i = 0;
	if (!ft_check_pipes(minishell))
	{
		perror("PIPE ERROR"); // to confirme what to do
		exit(2);
	}
	pipes = ft_split(minishell->clean_line, '|');
	if (pipes == NULL)
		free_minishell(minishell); // to check avec florence
	minishell->count_pipes = ft_find_pipes(pipes);
	minishell->cmd_table = ft_calloc(minishell->count_pipes, sizeof(t_cmd_table *));
	// if (minishell->cmd_table == NULL)
	// {
	// 	ft_free_double_char(pipes);
	// 	free_minishell(minishell); // to check avec florence
	// }
	minishell->child_pids = ft_calloc(minishell->count_pipes, sizeof(pid_t));
	// if (minishell->child_pids == NULL)
	// {
	// 	ft_free_double_char(pipes);
	// 	free_minishell(minishell); // to check avec florence
	// }
	ft_init_command_group((minishell->cmd_table), pipes, minishell);
	ft_free_double_char(pipes);
}

void	ft_parseur(t_shell *minishell)
{
	int		i;

	i = 0;
	ft_neg_inside_quote(minishell, i);
	ft_expand_dollar(minishell, i);
	ft_rev_neg_line(minishell);
	ft_parseur_quote(minishell);
	ft_split_pipe(minishell);
}
