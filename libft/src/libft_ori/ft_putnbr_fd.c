/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbeaudoi <gbeaudoi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/11 18:12:42 by gbeaudoi          #+#    #+#             */
/*   Updated: 2023/11/13 13:35:22 by gbeaudoi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_putnbr_fd(int n, int fd)
{
	unsigned int	i;

	if (n < 0)
	{
		ft_putchar_fd('-', fd);
		i = n * -1;
	}
	else
		i = n;
	if (i < 10) // if (i >= 0 && i < 10)
		ft_putchar_fd(i + 48, fd);
	else
	{
		ft_putnbr_fd(i / 10, fd);
		ft_putnbr_fd(i % 10, fd);
	}
}
