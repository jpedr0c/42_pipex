/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jocardos <jocardos@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/18 13:39:37 by jocardos          #+#    #+#             */
/*   Updated: 2022/05/19 14:47:01 by jocardos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	length(long n)
{
	size_t	number;

	number = 0;
	if (n < 0)
	{
		number++;
		n *= -1;
	}
	while (n > 9)
	{
		number++;
		n /= 10;
	}
	number++;
	return (number);
}

char	*ft_itoa(int n)
{
	char	*result;
	long	nbr;
	size_t	len;

	nbr = n;
	len = length(nbr);
	result = malloc(sizeof(char) * (len + 1));
	if (result == NULL)
		return (result);
	if (nbr < 0)
	{
		result[0] = '-';
		nbr *= -1;
	}
	result[len] = '\0';
	while (len > 0)
	{
		if (nbr / 10 < 1)
			break ;
		result[len-- - 1] = (nbr % 10) + 48;
		nbr /= 10;
	}
	result[len - 1] = nbr + 48;
	return (result);
}
