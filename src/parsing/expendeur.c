/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expendeur.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbeaudoi <gbeaudoi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/31 15:31:03 by gbeaudoi          #+#    #+#             */
/*   Updated: 2024/06/03 19:57:10 by gbeaudoi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	ft_find_value(char **to_join, t_shell *minishell, int i)
{
	int		j;
	char	*key;
	t_list	*list;
	t_env	*cur_content;

	j = 0;
	while (isalnum(minishell->clean_line[i + j]) || minishell->clean_line[i
		+ j] == '_')
		j++;
	key = ft_strndup(minishell->clean_line + i, j);
	if (key == NULL)
	{
		// reset
	}
	list = minishell->env_lst;
	while (list)
	{
		cur_content = (t_env *)list->content;
		if (!ft_strcmp(key, cur_content->key))
		{
			*to_join = ft_strjoin(cur_content->var, minishell->clean_line + i
					+ j);
			break ;
		}
		else
			*to_join = minishell->clean_line + i + j;
		list = list->next;
	}
}

static void	ft_dollar_option(t_shell *minishell, int i)
{
	char	*copy;
	char	*to_join;

	copy = ft_strndup(minishell->clean_line, i);
	if (ft_isdigit(minishell->clean_line[i + 1]) && minishell->clean_line[i
		+ 1] != '0')
		to_join = ft_strndup(minishell->clean_line + (i + 2),
				ft_strlen(minishell->clean_line) - i - 2);
	else if (minishell->clean_line[i + 1] == '0')
		to_join = ft_strjoin_no_free("miniminishell_Flo_&_G",
				minishell->clean_line + i + 2);
	else if (ft_isalpha(minishell->clean_line[i + 1]) || minishell->clean_line[i
		+ 1] == '_')
		ft_find_value(&to_join, minishell, i + 1);
	else if (minishell->clean_line[i + 1] == '\"' || minishell->clean_line[i
		+ 1] == '\'')
		to_join = minishell->clean_line + i + 1;
	else
	{
		minishell->clean_line[i] = minishell->clean_line[i] * -1;
		to_join = minishell->clean_line + i;
	}
	minishell->clean_line = ft_strjoin(copy, to_join);
}

void	ft_expand_dollar(t_shell *minishell, int i)
{
	int		flag_sgl;
	char	j;

	flag_sgl = 0;
	while (minishell->clean_line[i])
	{
		if (minishell->clean_line[i] == '$' && flag_sgl == 0)
		{
			ft_dollar_option(minishell, i);
			i = 0;
		}
		else
		{
			if (minishell->clean_line[i] == '\'')
			{
				if (flag_sgl == 1)
					flag_sgl -= 1;
				else
					flag_sgl = 1;
			}
			i++;
		}
	}
}