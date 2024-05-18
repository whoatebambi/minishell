/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbeaudoi <gbeaudoi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/07 20:43:22 by guillaumebe       #+#    #+#             */
/*   Updated: 2023/11/13 13:24:00 by gbeaudoi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memmove(void *dest, const void *src, size_t n)
{
	size_t			i;
	unsigned char	*bdest;
	unsigned char	*bsrc;

	bdest = (unsigned char *)dest;
	bsrc = (unsigned char *)src;
	i = 0;
	if (src == dest)
		return (dest);
	if (bdest > bsrc)
	{
		while (i++ < n)
			bdest[n - i] = bsrc[n - i];
	}
	else
	{
		i = 0;
		while (n > 0)
		{
			*(bdest + i) = *(bsrc + i);
			n--;
			i++;
		}
	}
	return (dest);
}
