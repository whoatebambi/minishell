/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_libft.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/21 20:27:20 by fcouserg          #+#    #+#             */
/*   Updated: 2024/11/05 02:15:31 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	safe_strcmp(char *s1, char *s2)
{
	if (!s1 || !s2)
		return (1);
	while (*s1 && *s2)
	{
		if (*s1 != *s2)
			return (1);
		s1++;
		s2++;
	}
	if (*s1 || *s2)
		return (1);
	return (0);
}

void	swap_env(t_list *a, t_list *b)
{
	void	*temp_content;

	temp_content = a->content;
	a->content = b->content;
	b->content = temp_content;
}
