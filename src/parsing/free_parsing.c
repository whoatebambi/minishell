/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_parsing.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/10 15:59:26 by gbeaudoi          #+#    #+#             */
/*   Updated: 2024/10/15 16:05:44 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_free_child(pid_t *i)
{
	if (!i)
		return ;
	free(i);
}

void	ft_check_strdup(char *str, int i, char **dest, int flag)
{
	if (flag == 1)
		*dest = ft_strndup(str, i);
	else 
		*dest = ft_strdup(str);
	if (*dest == NULL)
	{
		// free(*dest);
		// exitmsg(get_minishell(NULL), MERROR);
	}
}