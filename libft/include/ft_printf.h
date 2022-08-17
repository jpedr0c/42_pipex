/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jocardos <jocardos@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/01 15:36:02 by jocardos          #+#    #+#             */
/*   Updated: 2022/06/14 10:59:30 by jocardos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PRINTF_H
# define FT_PRINTF_H

# include "libft.h"
# include <stdarg.h>

int	ft_printf(const char *s, ...);
int	print_char(int c);
int	print_str(char *str);
int	print_nbr(int n);
int	print_nbr_unsig(unsigned int n);
int	show_percent(void);
int	print_ptr(unsigned long ptr);
int	print_hexa(unsigned int n, const char format);

#endif
