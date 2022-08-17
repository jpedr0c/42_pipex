/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcpy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jocardos <jocardos@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/11 10:24:00 by jocardos          #+#    #+#             */
/*   Updated: 2022/05/23 10:29:48 by jocardos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcpy(char *dst, const char *src, size_t dstsize)
{
	size_t	i;
	char	*ptr;

	ptr = (void *)0;
	if (src == NULL)
		return (*ptr);
	i = 0;
	while (src[i] != '\0')
		i++;
	if (dstsize == 0)
		return (i);
	i = 0;
	while (src[i] != '\0' && i < dstsize - 1)
	{
		dst[i] = src[i];
		i++;
	}
	dst[i] = '\0';
	i = '\0';
	while (src[i] != '\0')
		i++;
	return (i);
}
