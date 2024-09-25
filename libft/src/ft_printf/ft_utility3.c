/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_utility3.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcouserg <fcouserg@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/28 18:47:44 by gbeaudoi          #+#    #+#             */
/*   Updated: 2024/09/25 17:38:53 by fcouserg         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

// print all the different flags for the no dash path_char
void	ft_check_intdec(t_print *format, int k, int m, int i)
{
	int	sign;

	sign = ft_check_sign(i);
	if (sign)
	{
		ft_flag_sign(format);
		if ((format->zero && !format->precision_flag))
			ft_zero(format, m);
		else
			ft_space(format, m);
	}
	if (!sign)
	{
		if ((format->zero && !format->precision_flag))
		{
			ft_putchar(format, '-');
			ft_zero(format, m);
		}
		else
		{
			ft_space(format, m);
			ft_putchar(format, '-');
		}
	}
	ft_zero(format, k);
}

// verify the special case to print the int or not
void	ft_cond_print_int(t_print *format, int i)
{
	if (format->precision_flag)
	{
		if (format->precision == 0)
		{
			if (i == 0)
				return ;
		}
	}
	if (i == 0)
		ft_putchar(format, '0');
	else
		ft_putnbr(format, i);
}

void	ft_cond_print_unsint(t_print *format, unsigned int i)
{
	if (format->precision_flag)
	{
		if (format->precision == 0)
		{
			if (i == 0)
				return ;
		}
	}
	if (i == 0)
		ft_putchar(format, '0');
	else
		ft_putnbr_u(format, i);
}

void	ft_check_zero_hex(t_print *format, int flag, int m, unsigned int i)
{
	if (ft_flag_dash(format) == 'A')
	{
		ft_space(format, m);
		ft_flag_sign(format);
		ft_put_hashtag(format, flag, i);
		ft_zero(format, ft_count_prec_hex(format, ft_get_hex_len(i)));
	}
	else if (ft_flag_dash(format) == 'C')
	{
		ft_flag_sign(format);
		ft_put_hashtag(format, flag, i);
		if (i == 0)
			ft_zero(format, m);
		else
			ft_space(format, m);
		ft_zero(format, ft_count_prec_hex(format, ft_get_hex_len(i)));
	}
	else
	{
		ft_flag_sign(format);
		ft_put_hashtag(format, flag, i);
		ft_zero(format, m);
		ft_zero(format, ft_count_prec_hex(format, ft_get_hex_len(i)));
	}
}

// verify the special case to convert the unsigned int into hex
void	ft_cond_print_hex(t_print *format, unsigned int i, int flag)
{
	if (format->precision_flag)
	{
		if (format->precision == 0)
		{
			if (i == 0)
				return ;
		}
	}
	if (i == 0)
		ft_putchar(format, '0');
	else
		ft_hex_convert(format, i, flag);
}
