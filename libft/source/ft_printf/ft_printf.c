/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jocardos <jocardos@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/01 16:58:55 by jocardos          #+#    #+#             */
/*   Updated: 2022/06/06 11:34:56 by jocardos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	formats(va_list args, const char format)
{
	int	len_printed;

	len_printed = 0;
	if (format == 'c')
		len_printed += print_char(va_arg(args, int));
	else if (format == 's')
		len_printed += print_str(va_arg(args, char *));
	else if (format == 'p')
		len_printed += print_ptr(va_arg(args, unsigned long));
	else if (format == 'd' || format == 'i')
		len_printed += print_nbr(va_arg(args, int));
	else if (format == 'u')
		len_printed += print_nbr_unsig(va_arg(args, unsigned int));
	else if (format == 'x' || format == 'X')
		len_printed += print_hexa(va_arg(args, unsigned), format);
	else if (format == '%')
		len_printed += show_percent();
	return (len_printed);
}

int	ft_printf(const char *s, ...)
{
	va_list	args;
	int		len_printed;
	int		i;

	len_printed = 0;
	i = 0;
	va_start(args, s);
	while (s[i])
	{
		if (s[i] == '%')
		{
			len_printed += formats(args, s[i + 1]);
			i++;
		}
		else
			len_printed += print_char(s[i]);
		i++;
	}
	va_end(args);
	return (len_printed);
}
