/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expendeur.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbeaudoi <gbeaudoi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/31 15:31:03 by gbeaudoi          #+#    #+#             */
/*   Updated: 2024/11/04 14:05:19 by gbeaudoi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	ft_define_flag(int *flag_sgl, int *flag_dbl, t_shell *minishell,
		int *i)
{
	if (minishell->clean_line[*i] == '\'')
	{
		if (*flag_sgl == 1)
			*flag_sgl -= 1;
		else
			*flag_sgl = 1;
	}
	else
	{
		if (*flag_dbl == 1)
			*flag_dbl -= 1;
		else
			*flag_dbl = 1;
	}
	(*i)++;
}

static int	ft_helper_expand_dollar(t_shell *minishell, int *flag_sgl,
		int *flag_dbl, int *i)
{
	int	delim;

	delim = 0;
	if (minishell->clean_line[*i] == '\'' || minishell->clean_line[*i] == '\"')
		ft_define_flag(flag_sgl, flag_dbl, minishell, i);
	else if (minishell->clean_line[*i] == '<' && i > 0
		&& minishell->clean_line[*i - 1] == '<' && flag_dbl == 0)
	{
		delim = 1;
		(*i)++;
		while (minishell->clean_line[*i] == ' ')
			(*i)++;
	}
	else if (minishell->clean_line[*i] == ' ')
	{
		delim = 0;
		(*i)++;
	}
	else
		(*i)++;
	return (delim);
}

void	ft_expand_dollar(t_shell *minishell, int delim, int flag_sgl,
		int flag_dbl)
{
	char	*copy;
	int		i;

	i = 0;
	while (minishell->clean_line[i])
	{
		if (minishell->clean_line[i] == '$' && flag_sgl == 0 && delim == 0)
		{
			copy = ft_strndup(minishell->clean_line, i);
			if (copy == NULL)
				exitmsg(minishell, MERROR);
			ft_dollar_option(copy, minishell, i, flag_dbl);
			i = 0;
			flag_sgl = 0;
			flag_dbl = 0;
		}
		else
		{
			delim = ft_helper_expand_dollar(minishell, &flag_sgl, &flag_dbl,
					&i);
		}
	}
}
