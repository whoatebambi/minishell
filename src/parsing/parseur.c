/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parseur.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbeaudoi <gbeaudoi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/20 15:42:08 by gbeaudoi          #+#    #+#             */
/*   Updated: 2024/06/20 19:32:43 by gbeaudoi         ###   ########.fr       */
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
		if (command_table[i] == NULL)
		{
			// reset mini
		}
		command_table[i]->group_command = ft_strdup(pipes[i]);
		if (command_table[i] == NULL)
		{
			// reset mini
		}
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
	// if (pipes == NULL)
	// 	free_minishell(minishell); // to check avec florence
	minishell->count_pipes = ft_find_pipes(pipes);
	minishell->cmd_table = ft_calloc(minishell->count_pipes,
			sizeof(t_cmd_table *));
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
	t_node	*copy;
	int		j;
	t_redir	*copie;

	i = 0;
	ft_neg_inside_quote(minishell, i);
	ft_expand_dollar(minishell, i);
	ft_rev_neg_line(minishell);
	ft_parseur_quote(minishell);
	ft_split_pipe(minishell);
	// 	i = 0;
	// while (i < minishell->count_pipes)
	// {
	// 	ft_printf("[%d] GROUP COMMAND : %s\n", i,
	// 		minishell->cmd_table[i]->group_command);
	// 	copy = minishell->cmd_table[i]->nodes;
	// 	while (copy)
	// 	{
	// 		ft_printf("[string] : %s\n[token]  :% d | [redir] : % d | [quote] :% d\n ",
	// copy->string, copy->token, copy->redir, copy->quote);
	// 		copy = copy->next;
	// 	}
	// 	j = 0;
	// 	while (minishell->cmd_table[i]->cmd_args[j])
	// 	{
	// 		ft_printf("COMMAND ARG[%d] : %s\n", j,
	// 			minishell->cmd_table[i]->cmd_args[j]);
	// 		j++;
	// 	}
	// 	copie = minishell->cmd_table[i]->redirs_in;
	// 	while (copie)
	// 	{
	// 		ft_printf("redir_in type %d\n", copie->type);
	// 		ft_printf("redir_in redir %s\n", copie->redir_name);
	// 		ft_printf("redir_in quote %d\n", copie->quote);
	// 		copie = copie->next;
	// 	}
	// 	copie = minishell->cmd_table[i]->redirs_out;
	// 	while (copie)
	// 	{
	// 		ft_printf("redir_out type %d\n", copie->type);
	// 		ft_printf("redir_out redir %s\n", copie->redir_name);
	// 		ft_printf("redir_out quote %d\n", copie->quote);
	// 		copie = copie->next;
	// 	}
	// 	i++;
	// }
}
