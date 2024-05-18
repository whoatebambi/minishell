/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_print_intdec.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbeaudoi <gbeaudoi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/22 19:30:27 by gbeaudoi          #+#    #+#             */
/*   Updated: 2023/12/02 18:17:10 by gbeaudoi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

// get number of char from the int
int	ft_get_di_len(int i)
{
	int	len;

	len = 0;
	if (i == 0)
		return (1);
	while (i != 0)
	{
		i /= 10;
		len++;
	}
	return (len);
}

// classic putnbr but no - sign
void	ft_putnbr(t_print *format, int n)
{
	unsigned int	i;

	if (n < 0)
		i = n * -1;
	else
		i = n;
	if (i < 10) // if (i >= 0 && i < 10)
		ft_putchar(format, i + 48);
	else
	{
		ft_putnbr(format, i / 10);
		ft_putnbr(format, i % 10);
	}
}

// recalculate precision
int	ft_count_prec_id(t_print *format, int i)
{
	if (i == 0)
		return (format->precision);
	else if (format->precision > i)
		return (format->precision - i);
	else
		return (0);
}

// recalculate width based on flags and i and signs
int	ft_count_width_id(t_print *format, int k, int sign, int i)
{
	int	len;
	int	size;

	size = ft_get_di_len(i);
	len = 0;
	if (!sign)
		len++;
	else if (format->plus || format->space)
		len++;
	if (i == 0 && format->precision_flag && format->precision == 0)
		return (format->width + 1 - k - size - len);
	else if (format->width <= (k + size + len))
		return (0);
	else
		return (format->width - k - size - len);
}

void	ft_print_intdec(t_print *format)
{
	int	i;
	int	k;
	int	m;
	int	sign;

	i = va_arg(format->args, int);
	sign = ft_check_sign(i);
	k = ft_count_prec_id(format, ft_get_di_len(i));
	m = ft_count_width_id(format, k, sign, i);
	if (format->dash)
	{
		if (sign)
			ft_flag_sign(format);
		else
			ft_putchar(format, '-');
		ft_zero(format, k);
		ft_cond_print_int(format, i);
		ft_space(format, m);
	}
	else
	{
		ft_check_intdec(format, k, m, i);
		ft_cond_print_int(format, i);
	}
}
