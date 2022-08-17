/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jocardos <jocardos@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/16 11:03:27 by jocardos          #+#    #+#             */
/*   Updated: 2022/05/16 12:41:07 by jocardos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memchr(const void *s, int c, size_t n)
{
	size_t			i;
	unsigned char	*str;
	unsigned char	carac;

	i = 0;
	str = (unsigned char *)s;
	carac = (unsigned char )c;
	while (i < n)
	{
		if (str[i] == carac)
			return (str + i);
		i++;
	}
	return (0);
}
