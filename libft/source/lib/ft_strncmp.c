/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strncmp.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jocardos <jocardos@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/11 17:45:26 by jocardos          #+#    #+#             */
/*   Updated: 2022/05/23 10:30:47 by jocardos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	size_t			i;
	unsigned char	*pt_s1;
	unsigned char	*pt_s2;

	i = 0;
	pt_s1 = (unsigned char *)s1;
	pt_s2 = (unsigned char *)s2;
	if (n == 0)
		return (0);
	while (pt_s1[i] == pt_s2[i] && (i < n -1) && pt_s1[i])
		i++;
	return (pt_s1[i] - pt_s2[i]);
}
