/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jocardos <jocardos@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/06 17:28:28 by jocardos          #+#    #+#             */
/*   Updated: 2022/05/16 14:31:53 by jocardos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

char	*ft_strrchr(const char *s, int c)
{
	char	*pointer;

	pointer = 0;
	while (*s)
	{
		if (*s == (char)c)
			pointer = (char *)s;
		s++;
	}
	if (c == 0)
		return ((char *)s);
	return (pointer);
}
