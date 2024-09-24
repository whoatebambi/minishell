/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quote_handling.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcouserg <fcouserg@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/31 15:31:44 by gbeaudoi          #+#    #+#             */
/*   Updated: 2024/09/24 19:56:43 by fcouserg         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_parseur_helper(int i, int k, char *copy, t_shell *minishell)
{
	int		quote_count;
	char	j;

	quote_count = 0;
	while (copy[i])
	{
		if (copy[i] == '\"' || copy[i] == '\'')
		{
			j = copy[i];
			quote_count = 1;
			i++;
			while (copy[i] != j && copy[i])
				minishell->clean_line[k++] = copy[i++] * -1;
			if (copy[i] == j)
				quote_count -= 1;
			else if (!copy[i])
				break ;
		}
		else
			minishell->clean_line[k++] = copy[i];
		i++;
	}
	return (quote_count);
}

int	ft_parseur_quote(t_shell *minishell)
{
	int		i;
	int		k;
	int		quote_count;
	char	*copy;

	copy = ft_strdup(minishell->clean_line);
	// if (minishell->clean_line == NULL)
	// {
	// 	// clean the stuff
	// }
	ft_bzero(minishell->clean_line, ft_strlen(minishell->clean_line));
	i = 0;
	k = 0;
	quote_count = ft_parseur_helper(i, k, copy, minishell);
	if (quote_count == 1)
	{
		free(copy);
		free(minishell->clean_line);
		perror("Unclosed quotes");
		// ft_reset(minishell, all);
	}
	free(copy);
	return (1);
}

void	ft_neg_inside_quote(t_shell *minishell)
{
	int	i;
	
	i = 0;
	minishell->clean_line = ft_calloc(ft_strlen(minishell->line) + 1, sizeof(char));
	if (minishell->clean_line == NULL)
	{
		printf("malloc error\n");
		return ;
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
