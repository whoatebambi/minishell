/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: guillaumebeaudoin <guillaumebeaudoin@st    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/12 15:27:46 by guillaumebe       #+#    #+#             */
/*   Updated: 2023/11/12 17:30:42 by guillaumebe      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static size_t	ft_countsize(long num)
{
	size_t	size;

	size = 0;
	if (num < 0)
	{
		size = 1;
		num = num * -1;
	}
	if (num == 0)
		size = 1;
	else
	{
		while (num > 0)
		{
			num = num / 10;
			size++;
		}
	}
	return (size);
}

char	*ft_itoa(int n)
{
	int		size;
	int		sign;
	char	*itoa;
	long	nb;

	nb = (long)n;
	size = ft_countsize(nb);
	sign = 0;
	if (n < 0)
	{
		sign = 1;
		nb = nb * -1;
	}
	itoa = (char *)malloc(sizeof(char) * (size + 1));
	if (itoa == NULL)
		return (NULL);
	itoa[size] = '\0';
	if (sign)
		itoa[0] = '-';
	while (size > sign)
	{
		itoa[--size] = nb % 10 + 48;
		nb = nb / 10;
	}
	return (itoa);
}
