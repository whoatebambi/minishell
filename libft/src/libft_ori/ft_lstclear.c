/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstclear.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbeaudoi <gbeaudoi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/13 18:24:18 by gbeaudoi          #+#    #+#             */
/*   Updated: 2023/11/13 19:11:22 by gbeaudoi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstclear(t_list **lst, void (*del)(void *))
{
	t_list	*temp_node;

	if (!lst || !(*lst) || !del)
		return ;
	while ((*lst))
	{
		temp_node = (*lst)->next;
		ft_lstdelone(*lst, del);
		(*lst) = temp_node;
	}
	lst = NULL;
}
