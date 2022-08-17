/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_hexa.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jocardos <jocardos@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/01 16:11:44 by jocardos          #+#    #+#             */
/*   Updated: 2022/06/06 11:33:35 by jocardos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	hexa_len(unsigned int n)
{
	int	len;

	len = 0;
	while (n != 0)
	{
		len++;
		n /= 16;
	}
	return (len);
}

void	put_hexa(unsigned int n, const char format)
{
	if (n >= 16)
	{
		put_hexa((n / 16), format);
		put_hexa((n % 16), format);
	}
	else
	{
		if (n <= 9)
			ft_putchar_fd((n + 48), 1);
		else
		{
			if (format == 'x')
				ft_putchar_fd(((n - 10) + 97), 1);
			if (format == 'X')
				ft_putchar_fd(((n - 10) + 65), 1);
		}
	}
}

int	print_hexa(unsigned int n, const char format)
{
	if (n == 0)
		return (print_char(48));
	else
		put_hexa(n, format);
	return (hexa_len(n));
}
