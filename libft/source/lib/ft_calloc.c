/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jocardos <jocardos@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/09 14:03:36 by jocardos          #+#    #+#             */
/*   Updated: 2022/05/23 10:42:39 by jocardos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_calloc(size_t count, size_t size)
{
	size_t	total;
	void	*memory;

	if (count == 0 || size == 0)
	{
		count = 1;
		size = 1;
	}
	total = count * size;
	if (total / count != size)
		return (NULL);
	memory = malloc(total);
	if (memory)
		ft_bzero(memory, total);
	return (memory);
}
