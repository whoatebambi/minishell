/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbeaudoi <gbeaudoi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/20 16:29:44 by gbeaudoi          #+#    #+#             */
/*   Updated: 2023/12/23 14:57:25 by gbeaudoi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PRINTF_H
# define FT_PRINTF_H

# include <stdarg.h>
# include <stdlib.h>
# include <unistd.h>

typedef struct s_print
{
	va_list	args;
	int		width;
	int		precision;
	int		zero;
	int		dot;
	int		dash;
	int		hastag;
	int		plus;
	int		count;
	int		width_flag;
	int		precision_flag;
	int		space;
	int		no_flags;
}			t_print;

int			ft_printf(const char *str, ...);
t_print		*ft_initialize(t_print *format);
void		ft_putchar(t_print *format, char c);
void		ft_zero(t_print *format, int i);
void		ft_space(t_print *format, int i);
void		ft_flag_sign(t_print *format);
char		ft_flag_dash(t_print *format);
char		ft_flag_prec_int(t_print *format);
int			ft_isdigit(int c);
int			ft_check_sign(int i);
void		ft_check_zero(t_print *format, int m, int sign);
int			ft_len_digit(t_print *format, const char *str, int i);
void		ft_convert(t_print *format, const char *str, int i);
int			ft_flags(t_print *format, const char *str, int i);
void		ft_print_char(t_print *format);
int			ft_count_width(t_print *format, int k);
int			ft_count_prec(t_print *format, char *str);
void		ft_print_str(t_print *format);
size_t		ft_strlen(const char *str);
int			ft_count_width_pointer(t_print *format, unsigned long p);
void		ft_pointer_convert(t_print *format, unsigned long p);
void		ft_print_pointer(t_print *format);
int			ft_get_di_len(int i);
void		ft_putnbr(t_print *format, int n);
int			ft_count_prec_id(t_print *format, int i);
int			ft_count_width_id(t_print *format, int k, int sign, int i);
void		ft_print_intdec(t_print *format);
int			ft_get_u_len(unsigned int i);
void		ft_putnbr_u(t_print *format, unsigned int n);
int			ft_count_prec_u(t_print *format, unsigned int i);
int			ft_count_width_u(t_print *format, int k, unsigned int i);
void		ft_print_uns_dec(t_print *format);
int			ft_get_hex_len(unsigned int i);
int			ft_count_width_hex(t_print *format, int k, unsigned int i);
int			ft_count_prec_hex(t_print *format, int i);
void		ft_hex_convert(t_print *format, unsigned int i, int flag);
void		ft_put_hashtag(t_print *format, int flag, unsigned int i);
void		ft_print_hex(t_print *format, int flag);
void		ft_check_str(t_print *format, char *str, int k);
void		ft_check_zero_hex(t_print *format, int flag, int m, unsigned int i);
void		ft_check_pointer(t_print *format, unsigned long p);
void		ft_check_intdec(t_print *format, int k, int m, int i);
int			ft_len_pointer(unsigned long p);
void		ft_cond_print_int(t_print *format, int i);
void		ft_cond_print_unsint(t_print *format, unsigned int i);
void		ft_cond_print_hex(t_print *format, unsigned int i, int flag);

#endif