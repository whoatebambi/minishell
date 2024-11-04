/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expendeur_helper.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbeaudoi <gbeaudoi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/17 16:02:51 by gbeaudoi          #+#    #+#             */
/*   Updated: 2024/11/04 15:54:29 by gbeaudoi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_find_value(t_shell *minishell, char **to_join, int i, int j)
{
	char	*key;
	t_env	*list;

	while (ft_isalnum(minishell->clean_line[i + j]) || minishell->clean_line[i
			+ j] == '_')
		j++;
	key = ft_strndup(minishell->clean_line + i, j);
	if (!key)
		exitmsg(minishell, MERROR);
	list = minishell->env;
	while (list)
	{
		if (*to_join)
			free(*to_join);
		if (!ft_strcmp(key, list->key))
		{
			*to_join = ft_strjoin_no_free(list->value, minishell->clean_line + i
					+ j);
			break ;
		}
		else
			*to_join = ft_strdup(minishell->clean_line + i + j);
		list = list->next;
	}
	free(key);
}

static void	ft_join_clean_line(t_shell *minishell, char *copy, char *to_join)
{
	if (to_join)
	{
		free(minishell->clean_line);
		minishell->clean_line = ft_strjoin(copy, to_join);
		if (!minishell->clean_line)
		{
			free(copy);
			free(to_join);
			exitmsg(minishell, MERROR);
		}
		free(to_join);
		to_join = NULL;
	}
	else
	{
		free(to_join);
		free(copy);
		to_join = NULL;
	}
}

void	ft_dollar_option(char *copy, t_shell *minishell, int i, int flag_dbl)
{
	char	*to_join;

	to_join = NULL;
	if (ft_isdigit(minishell->clean_line[i + 1]) && minishell->clean_line[i
			+ 1] != '0')
		to_join = ft_strdup(minishell->clean_line + (i + 2));
	else if (minishell->clean_line[i + 1] == '0')
		to_join = ft_strjoin_no_free("miniminishell_Flo_&_G",
				minishell->clean_line + i + 2);
	else if (ft_isalpha(minishell->clean_line[i + 1]) || minishell->clean_line[i
			+ 1] == '_')
		ft_find_value(minishell, &to_join, i + 1, 0);
	else if (minishell->clean_line[i + 1] == '?')
		to_join = ft_strjoin(ft_itoa(minishell->tmpexcode),
				minishell->clean_line + i + 2);
	else if ((minishell->clean_line[i + 1] == '\"' && flag_dbl == 0)
		|| minishell->clean_line[i + 1] == '\'')
		to_join = ft_strdup(minishell->clean_line + i + 1);
	else
		minishell->clean_line[i] = minishell->clean_line[i] * -1;
	ft_join_clean_line(minishell, copy, to_join);
}
