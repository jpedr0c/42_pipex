/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strmapi.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jocardos <jocardos@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/17 10:52:10 by jocardos          #+#    #+#             */
/*   Updated: 2022/05/17 13:49:53 by jocardos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strmapi(char const *s, char (*f)(unsigned int, char))
{
	char	*str;
	size_t	i;
	size_t	len;

	if (!s)
		return ((void *)0);
	len = ft_strlen(s);
	str = malloc(len + 1);
	if (!str)
		return ((void *)0);
	i = 0;
	while (s[i])
	{
		str[i] = f(i, *(s + i));
		i++;
	}
	str[i] = '\0';
	return (str);
}
