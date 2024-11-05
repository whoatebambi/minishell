/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expendeur.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/31 15:31:03 by gbeaudoi          #+#    #+#             */
/*   Updated: 2024/11/05 00:25:49 by codespace        ###   ########.fr       */
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

static void	ft_helper_expand_dollar(t_shell *minishell, int *delim,
		int *flag_dbl, int *i)
{
	if (minishell->clean_line[*i] == '<' && *i > 0 && minishell->clean_line[*i
			- 1] == '<' && *flag_dbl == 0)
	{
		*delim = 1;
		(*i)++;
		while (minishell->clean_line[*i] == ' ')
			(*i)++;
	}
	else if (minishell->clean_line[*i] == ' ')
	{
		*delim = 0;
		(*i)++;
	}
	else
	{
		*delim = 0;
		(*i)++;
	}
}

void	ft_reset_flag_and_i(int *flag_sgl, int *flag_dbl, int *i)
{
	*flag_dbl = 0;
	*flag_sgl = 0;
	*i = 0;
}

void	ft_expand_dollar(t_shell *minishell, int flag_sgl, int flag_dbl)
{
	char	*copy;
	int		i;
	int		delim;

	i = 0;
	delim = 0;
	copy = NULL;
	while (minishell->clean_line[i])
	{
		if (minishell->clean_line[i] == '$' && flag_sgl == 0 && delim == 0)
		{
			copy = ft_strndup(minishell->clean_line, i);
			if (copy == NULL)
				exitmsg(minishell, "Malloc error");
			ft_dollar_option(copy, minishell, i, flag_dbl);
			ft_reset_flag_and_i(&flag_sgl, &flag_dbl, &i);
			if (copy)
				free(copy);
		}
		else if (minishell->clean_line[i] == '\''
			|| minishell->clean_line[i] == '\"')
			ft_define_flag(&flag_sgl, &flag_dbl, minishell, &i);
		else
			ft_helper_expand_dollar(minishell, &delim, &flag_dbl, &i);
	}
}
