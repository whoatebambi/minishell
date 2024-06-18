/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expendeur_helper.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbeaudoi <gbeaudoi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/17 16:02:51 by gbeaudoi          #+#    #+#             */
/*   Updated: 2024/06/17 16:12:31 by gbeaudoi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


// void	ft_find_value(char **to_join, t_shell *minishell, int i)
// {
// 	int		j;
// 	char	*key;
// 	t_list	*list;
// 	t_env	*cur_content;

// 	j = 0;
// 	while (isalnum(minishell->clean_line[i + j]) || minishell->clean_line[i
// 		+ j] == '_')
// 		j++;
// 	ft_check_strdup(minishell->clean_line + i, j, &key);
// 	list = minishell->env_lst;
// 	while (list)
// 	{
// 		cur_content = (t_env *)list->content;
// 		if (!ft_strcmp(key, cur_content->key))
// 		{
// 			*to_join = ft_strjoin_no_free(cur_content->var,
// 					minishell->clean_line + i + j);
// 			break ;
// 		}
// 		else
// 			*to_join = minishell->clean_line + i + j;
// 		list = list->next;
// 	}
// 	free(key);
// }
	// key = ft_strndup(minishell->clean_line + i, j);
	// if (key == NULL)
	// {
	// 	// reset
	// }