/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbeaudoi <gbeaudoi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/17 14:50:37 by gbeaudoi          #+#    #+#             */
/*   Updated: 2023/12/02 17:03:52 by gbeaudoi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

t_print	*ft_initialize(t_print *format)
{
	format->width = 0;
	format->precision = 0;
	format->zero = 0;
	format->dot = 0;
	format->dash = 0;
	format->hastag = 0;
	format->plus = 0;
	format->count = 0;
	format->width_flag = 0;
	format->precision_flag = 0;
	format->space = 0;
	format->no_flags = 0;
	return (format);
}

int	ft_printf(const char *str, ...)
{
	int		i;
	t_print	*format;
	int		len;

	format = (t_print *)malloc(sizeof(t_print));
	if (format == NULL)
		return (-1);
	ft_initialize(format);
	va_start(format->args, str);
	i = -1;
	len = 0;
	while (str[++i] != '\0')
	{
		if (str[i] == '%')
		{
			i = ft_flags(format, str, i + 1);
			len += format->count;
			ft_initialize(format);
		}
		else
			len += write(1, &str[i], 1);
	}
	va_end(format->args);
	free(format);
	return (len);
}
