/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_print_hex.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbeaudoi <gbeaudoi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/24 17:12:48 by gbeaudoi          #+#    #+#             */
/*   Updated: 2023/12/02 18:04:29 by gbeaudoi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

// tells the # of character that will be printed
int	ft_get_hex_len(unsigned int i)
{
	int	len;

	len = 0;
	if (i == 0)
		return (1);
	// if (i < 0)
	// 	len = 1;
	while (i != 0)
	{
		i /= 16;
		len++;
	}
	return (len);
}

// return the width to be used if in play
// taking in consideration all the different flags
int	ft_count_width_hex(t_print *format, int k, unsigned int i)
{
	int	len;
	int	size;

	size = ft_get_hex_len(i);
	len = 0;
	if (format->plus && format->space)
		len++;
	else if (format->plus || format->space)
		len++;
	else if (format->hastag)
		len += 2;
	else if (i == 0 && format->precision_flag && format->precision == 0)
		return (format->width + 1 - k - size - len);
	if (format->width <= k + size + len)
		return (0);
	else
		return (format->width - k - size - len);
}

// return the precision to be used if in play
// taking in consideration all the different flags
int	ft_count_prec_hex(t_print *format, int i)
{
	if (i == 0)
		return (format->precision);
	if (format->precision > i)
		return (format->precision - i);
	else
		return (0);
}

// print from dec to hex
void	ft_hex_convert(t_print *format, unsigned int i, int flag)
{
	char	*low;
	char	*up;

	low = "0123456789abcdef";
	up = "0123456789ABCDEF";
	if (flag == 1)
	{
		if (i >= 16)
			ft_hex_convert(format, i / 16, flag);
		format->count += write(1, &low[i % 16], 1);
	}
	else
	{
		if (i >= 16)
			ft_hex_convert(format, i / 16, flag);
		format->count += write(1, &up[i % 16], 1);
	}
}

void	ft_print_hex(t_print *format, int flag)
{
	unsigned int	i;
	int				k;
	int				m;

	i = va_arg(format->args, unsigned int);
	k = ft_count_prec_hex(format, ft_get_hex_len(i));
	m = ft_count_width_hex(format, k, i);
	if (format->dash)
	{
		ft_flag_sign(format);
		ft_put_hashtag(format, flag, i);
		ft_zero(format, k);
		ft_cond_print_hex(format, i, flag);
		ft_space(format, m);
	}
	else
	{
		ft_check_zero_hex(format, flag, m, i);
		ft_cond_print_hex(format, i, flag);
	}
}
