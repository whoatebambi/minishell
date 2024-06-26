/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expendeur.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbeaudoi <gbeaudoi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/31 15:31:03 by gbeaudoi          #+#    #+#             */
/*   Updated: 2024/06/17 16:35:13 by gbeaudoi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_find_value(char **to_join, t_shell *minishell, int i)
{
	int		j;
	char	*key;
	t_list	*list;
	t_env	*cur_content;

	j = 0;
	while (isalnum(minishell->clean_line[i + j]) || minishell->clean_line[i
		+ j] == '_')
		j++;
	ft_check_strdup(minishell->clean_line + i, j, &key, 1);
	list = minishell->env_lst;
	while (list)
	{
		cur_content = (t_env *)list->content;
		if (!ft_strcmp(key, cur_content->key))
		{
			*to_join = ft_strjoin_no_free(cur_content->var,
					minishell->clean_line + i + j);
			break ;
		}
		else
			ft_check_strdup(minishell->clean_line + i + j, 0, to_join, 0);
		list = list->next;
	}
	free(key);
}

static void	ft_dollar_option(char *copy, t_shell *minishell, int i,
		int flag_dbl)
{
	char	*to_join;
	char	*dup;

	// dup = ft_strdup()
	if (ft_isdigit(minishell->clean_line[i + 1]) && minishell->clean_line[i
		+ 1] != '0')
	{
		to_join = ft_strdup(minishell->clean_line + (i + 2));
		// if (mini... == NULL)
		// to be free;
	}
	else if (minishell->clean_line[i + 1] == '0')
	{
		to_join = ft_strjoin_no_free("miniminishell_Flo_&_G",
				minishell->clean_line + i + 2);
		// if (mini... == NULL)
		// to be free;
	}
	else if (ft_isalpha(minishell->clean_line[i + 1]) || minishell->clean_line[i
		+ 1] == '_')
		ft_find_value(&to_join, minishell, i + 1);
	else if (minishell->clean_line[i + 1] == '?')
	{
		to_join = ft_strjoin(ft_itoa(minishell->exit_code),
				minishell->clean_line + i + 2);
		// if (mini... == NULL)
		// to be free;
	}
	else if ((minishell->clean_line[i + 1] == '\"' && flag_dbl == 0)
		|| minishell->clean_line[i + 1] == '\'')
	{
		to_join = ft_strdup(minishell->clean_line + i + 1);
		// if (mini... == NULL)
		// to be free;
	}
	else
	{
		minishell->clean_line[i] = minishell->clean_line[i] * -1;
		to_join = ft_strdup(minishell->clean_line + i);
		// if (mini... == NULL)
		// to be free;
	}
	if (to_join)
	{
		free(minishell->clean_line);
		minishell->clean_line = ft_strjoin(copy, to_join);
		// if (mini... == NULL)
		// to be free;
	}
	// free(to_join);
}
static void	ft_define_flag(int *flag)
{
	if (*flag == 1)
		*flag -= 1;
	else
		*flag = 1;
}

void	ft_expand_dollar(t_shell *minishell, int i)
{
	int		flag_sgl;
	int		delim;
	char	*copy;
	int		flag_dbl;

	flag_sgl = 0;
	flag_dbl = 0;
	while (minishell->clean_line[i])
	{
		if (minishell->clean_line[i] == '$' && flag_sgl == 0 && delim == 0)
		{
			copy = ft_strndup(minishell->clean_line, i);
			if (copy == NULL)
			{
				// reset
			}
			ft_dollar_option(copy, minishell, i, flag_dbl);
			i = 0;
			flag_sgl = 0;
			flag_dbl = 0;
		}
		else if (minishell->clean_line[i] == '\'')
		{
			ft_define_flag(&flag_sgl);
			i++;
		}
		else if (minishell->clean_line[i] == '\"')
		{
			ft_define_flag(&flag_dbl);
			i++;
		}
		else if (minishell->clean_line[i] == '<' && i > 0
			&& minishell->clean_line[i - 1] == '<' && flag_dbl == 0)
		{
			delim = 1;
			i++;
			while (minishell->clean_line[i] == ' ')
				i++;
		}
		else if (minishell->clean_line[i] == ' ')
		{
			delim = 0;
			i++;
		}
		else
			i++;
	}
}
