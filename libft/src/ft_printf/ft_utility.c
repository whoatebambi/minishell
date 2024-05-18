/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_utility.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbeaudoi <gbeaudoi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/22 15:45:04 by gbeaudoi          #+#    #+#             */
/*   Updated: 2023/12/02 18:34:38 by gbeaudoi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

// print the character desired and keep track of the byte
void	ft_putchar(t_print *format, char c)
{
	format->count += write(1, &c, 1);
}

// print # zero and track the byte
void	ft_zero(t_print *format, int i)
{
	int	k;

	k = -1;
	while (++k < i)
		format->count += write(1, "0", 1);
}

// print # ' ' and track the byte
void	ft_space(t_print *format, int i)
{
	int	k;

	k = -1;
	while (++k < i)
		format->count += write(1, " ", 1);
}

// print either a + or space based on flags and track the byte
void	ft_flag_sign(t_print *format)
{
	if (format->plus && format->space)
		format->count += write(1, "+", 1);
	else if (format->plus && !(format->space))
		format->count += write(1, "+", 1);
	else if (!(format->plus) && format->space)
		format->count += write(1, " ", 1);
}

/* return a letter based on the dash, zero and precision flag
which allows to then asses how to return what */
char	ft_flag_dash(t_print *format)
{
	if (format->dash && format->zero)
		return ('A');
	else if (format->dash && !(format->zero))
		return ('A');
	else if (!(format->dash) && format->zero && !format->precision_flag)
		return ('B');
	else if (!(format->dash) && format->zero && format->precision_flag)
		return ('A');
	else
		return ('C');
}
