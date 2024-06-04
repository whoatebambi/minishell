/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quote_handling.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbeaudoi <gbeaudoi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/31 15:31:44 by gbeaudoi          #+#    #+#             */
/*   Updated: 2024/06/03 18:43:58 by gbeaudoi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_parseur_quote(t_shell *minishell)
{
	int		i;
	char	j;
	int		k;
	int		quote_count;
	int		flag_single;
	char	*copy;

	copy = ft_strdup(minishell->clean_line);
	ft_bzero(minishell->clean_line, ft_strlen(minishell->clean_line));
	i = 0;
	k = 0;
	quote_count = 0;
	while (copy[i])
	{
		if (copy[i] == '\"' || copy[i] == '\'')
		{
			j = copy[i];
			quote_count = 1;
			i++;
			while (copy[i] != j && copy[i])
			{
				minishell->clean_line[k] = copy[i] * -1;
				i++;
				k++;
			}
			if (copy[i] == j)
				quote_count -= 1;
			else if (!copy[i])
				break ;
		}
		else
		{
			minishell->clean_line[k] = copy[i];
			k++;
		}
		i++;
	}
	if (quote_count == 1)
	{
		free(minishell->clean_line);
		perror("Unclosed quotes");
		// ft_reset(minishell, all);
	}
	// while (minishell->clean_line[i])
	// 	printf("%c\n", minishell->clean_line[i++]);
	return (1);
}

void	ft_neg_inside_quote(t_shell *minishell, int i)
{
	minishell->clean_line = ft_calloc(ft_strlen(minishell->line) + 1,
			sizeof(char));
	if (minishell->clean_line == NULL)
	{
		// clean the stuff
	}
	while (minishell->line[i])
	{
		if (minishell->line[i] == '\"')
		{
			minishell->clean_line[i] = minishell->line[i];
			i++;
			while (minishell->line[i] != '\"' && minishell->line[i])
			{
				if (minishell->line[i] == '\'')
					minishell->clean_line[i] = minishell->line[i] * -1;
				else
					minishell->clean_line[i] = minishell->line[i];
				i++;
			}
		}
		if (minishell->line[i])
			minishell->clean_line[i] = minishell->line[i];
		i++;
	}
}
