/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quote_handling.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbeaudoi <gbeaudoi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/31 15:31:44 by gbeaudoi          #+#    #+#             */
/*   Updated: 2024/11/05 12:16:01 by gbeaudoi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_check_syntax(t_node *nodes, t_shell *minishell)
{
	t_node	*copy;

	copy = nodes;
	while (copy)
	{
		if (copy->token == 1 && copy->next && copy->next->token == 1)
		{
			minishell->excode = 2;
			printf("syntax error near unexpected token\n");
			return (0);
		}
		if (copy->token == 1 && !copy->next)
		{
			minishell->excode = 2;
			printf("syntax error near unexpected token\n");
			return (0);
		}
		copy = copy->next;
	}
	return (1);
}

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
	if (copy == NULL)
		exitmsg(minishell, "Malloc error");
	ft_bzero(minishell->clean_line, ft_strlen(minishell->clean_line));
	i = 0;
	k = 0;
	quote_count = ft_parseur_helper(i, k, copy, minishell);
	free(copy);
	if (quote_count == 1)
	{
		minishell->excode = 0;
		exitmsg(minishell, "Unclosed quotes");
	}
	return (1);
}

static void	ft_helper_neg_inside_quote(t_shell *minishell, int *i)
{
	minishell->clean_line[*i] = minishell->line[*i];
	(*i)++;
	while (minishell->line[*i] && minishell->line[*i] != '\"')
	{
		if (minishell->line[*i] == '\'')
			minishell->clean_line[*i] = minishell->line[*i] * -1;
		else
			minishell->clean_line[*i] = minishell->line[*i];
		(*i)++;
	}
	if (minishell->line[*i] == '\"')
	{
		minishell->clean_line[*i] = minishell->line[*i];
		(*i)++;
	}
}

void	ft_neg_inside_quote(t_shell *minishell)
{
	int	i;

	i = 0;
	minishell->clean_line = ft_calloc(ft_strlen(minishell->line) + 1,
			sizeof(char));
	if (minishell->clean_line == NULL)
		exitmsg(minishell, "Malloc error");
	while (minishell->line[i])
	{
		if (minishell->line[i] == '\"')
		{
			ft_helper_neg_inside_quote(minishell, &i);
		}
		else
		{
			minishell->clean_line[i] = minishell->line[i];
			i++;
		}
	}
}
