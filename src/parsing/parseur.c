/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parseur.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbeaudoi <gbeaudoi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/20 15:42:08 by gbeaudoi          #+#    #+#             */
/*   Updated: 2024/06/04 15:23:43 by gbeaudoi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// static int	ft_split_commands(t_shell *minishell)
// {
// 	int	i;
// 	int	j;
// 	int	k;

// 	i = 0;
// 	j = 0;
// 	k = 0;
// 	minishell->commands = (t_command **)malloc(sizeof(t_command *)
// 			* minishell->pipes->nb_command);
// 	if (minishell->commands == NULL)
// 	{
// 		// reset mini
// 	}
// 	while (i < minishell->pipes->nb_command)
// 	{
// 		minishell->commands[i]->full_command = minishell->pipes->tab_commands[0];
// 		minishell->commands[i]->split_command = ft_split(minishell->commands[i]->full_command,
// 				" ");
// 		if (minishell->pipes->tab_commands == NULL)
// 		{
// 			// reset mini
// 		}
// 		minishell->commands[i]->split_command[j]
// 	}
// }

// static int	ft_find_pipes(t_shell *minishell)
// {
// 	int	i;
// 	int	count;

// 	i = 0;
// 	count = 0;
// 	while (minishell->clean_line[i])
// 	{
// 		if (minishell->clean_line[i] == "|")
// 			count++;
// 		i++;
// 	}
// 	return (count);
// }

// static int	ft_split_pipe(t_shell *minishell)
// {
// 	minishell->pipes = malloc(sizeof(t_pipes));
// 	if (minishell->pipes == NULL)
// 	{
// 		// reset mini
// 	}
// 	minishell->pipes->nb_pipe = ft_find_pipes(minishell);
// 	minishell->pipes->nb_command = minishell->pipes->nb_pipe + 1;
// 	minishell->pipes->tab_commands = ft_split(minishell->clean_line, "|");
// 	if (minishell->pipes->tab_commands == NULL)
// 	{
// 		// reset mini
// 	}
// 	// DO WE WANT TO MANAGE IF NO COMMAND AFTER A PIPE ? AKA OPENING A HEREDOC TemP FOR A COMMAND TO BE WRITEN
// }

void	ft_parseur(t_shell *minishell)
{
	int i;

	i = 0;
	ft_neg_inside_quote(minishell, i);
	ft_expand_dollar(minishell, i);
	ft_rev_neg_line(minishell);
	ft_parseur_quote(minishell);
	ft_rev_neg_line(minishell);
	ft_printf("%s\n", minishell->clean_line);
}
