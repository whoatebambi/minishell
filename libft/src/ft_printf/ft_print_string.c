/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_print_string.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbeaudoi <gbeaudoi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/21 19:03:44 by gbeaudoi          #+#    #+#             */
/*   Updated: 2024/02/14 16:11:40 by gbeaudoi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	ft_count_width(t_print *format, int k)
{
	if (format->width <= k)
		return (0);
	else
		return (format->width - k);
}

int	ft_count_prec(t_print *format, char *str)
{
	int	len;

	if (str == 0)
		len = 6;
	else
		len = ft_strlen(str);
	if (format->precision == 0 && format->precision_flag)
		return (0);
	else if (format->precision == 0)
		return (len);
	else if (len <= format->precision)
		return (len);
	else
		return (format->precision);
}

void	ft_check_str(t_print *format, char *str, int k)
{
	int	i;

	i = -1;
	if (str == NULL && format->precision < 6 && format->precision_flag)
		return ;
	if (str == NULL)
		format->count += write(1, "(null)", 6);
	else
	{
		while (str[++i] && i < k)
			ft_putchar(format, str[i]);
	}
}

void	ft_print_str(t_print *format)
{
	char	*str;
	int		k;
	int		m;

	str = va_arg(format->args, char *);
	k = ft_count_prec(format, str);
	m = ft_count_width(format, k);
	if (format->precision_flag && format->precision == 0 && !format->width_flag)
		return ;
	if (format->dash)
	{
		ft_check_str(format, str, k);
		ft_space(format, m);
	}
	else
	{
		ft_space(format, m);
		ft_check_str(format, str, k);
	}
}
