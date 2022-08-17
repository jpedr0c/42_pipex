/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jocardos <jocardos@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/31 17:00:41 by jocardos          #+#    #+#             */
/*   Updated: 2022/06/06 11:33:32 by jocardos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	print_char(int c)
{
	ft_putchar_fd(c, 1);
	return (1);
}

int	print_str(char *str)
{
	int	i;

	i = 0;
	if (!str)
	{
		ft_putstr_fd("(null)", 1);
		return (6);
	}
	while (str[i] != '\0')
	{
		ft_putchar_fd(str[i], 1);
		i++;
	}
	return (i);
}

int	print_nbr(int n)
{
	int		len_nbr;
	char	*convert;

	len_nbr = 0;
	convert = ft_itoa(n);
	len_nbr = print_str(convert);
	free(convert);
	return (len_nbr);
}

int	print_nbr_unsig(unsigned int n)
{
	int		put_len;
	char	*convert;

	put_len = 0;
	if (n == 0)
		put_len += write(1, "0", 1);
	else
	{
		convert = ft_utoa(n);
		put_len += print_str(convert);
		free(convert);
	}
	return (put_len);
}

int	show_percent(void)
{
	ft_putstr_fd("%", 1);
	return (1);
}
