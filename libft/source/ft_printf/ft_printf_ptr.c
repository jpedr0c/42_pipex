/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_ptr.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jocardos <jocardos@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/02 16:09:32 by jocardos          #+#    #+#             */
/*   Updated: 2022/06/06 11:33:34 by jocardos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	ptr_len(unsigned long n)
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

void	put_ptr(unsigned long n)
{
	if (n >= 16)
	{
		put_ptr(n / 16);
		put_ptr(n % 16);
	}
	else
	{
		if (n <= 9)
			ft_putchar_fd((n + 48), 1);
		else
			ft_putchar_fd(((n - 10) + 97), 1);
	}
}

int	print_ptr(unsigned long ptr)
{
	int	len_ptr;

	len_ptr = 0;
	len_ptr += print_str("0x");
	if (ptr == 0)
		len_ptr += print_char(48);
	else
	{
		put_ptr(ptr);
		len_ptr += ptr_len(ptr);
	}
	return (len_ptr);
}
