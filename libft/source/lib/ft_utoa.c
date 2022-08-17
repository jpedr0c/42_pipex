/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_utoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jocardos <jocardos@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/31 16:29:33 by jocardos          #+#    #+#             */
/*   Updated: 2022/06/02 12:31:59 by jocardos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static	size_t	length(unsigned int n)
{
	size_t	i;

	i = 0;
	while (n != 0)
	{
		i++;
		n /= 10;
	}
	return (i);
}

char	*ft_utoa(unsigned int n)
{
	size_t	len;
	char	*result;

	len = length(n);
	result = malloc(sizeof(char) * (len + 1));
	if (!result)
		return (0);
	result[len] = '\0';
	while (n != 0)
	{
		result[len - 1] = (n % 10) + 48;
		n /= 10;
		len--;
	}
	return (result);
}
