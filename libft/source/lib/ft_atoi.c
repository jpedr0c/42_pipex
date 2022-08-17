/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jocardos <jocardos@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/16 12:41:39 by jocardos          #+#    #+#             */
/*   Updated: 2022/05/17 16:45:38 by jocardos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static	int	over(int signal)
{
	if (signal > 0)
		return (-1);
	else
		return (0);
}

int	ft_atoi(const char *str)
{
	int		signal;
	long	result;
	size_t	i;
	long	check;

	i = 0;
	signal = 1;
	result = 0;
	while (str[i] && (str[i] == ' ' || str[i] == '\n' || str[i] == '\t'
			||str[i] == '\v' || str[i] == '\f' || str[i] == '\r'))
		i++;
	if (str[i] == '-' || str[i] == '+')
	{
		if (str[i++] == '-')
			signal = -1;
	}
	while (str[i] && str[i] >= 48 && str[i] <= 57)
	{
		check = result;
		result = result * 10 + (str[i++] - 48);
		if (result < check)
			return (over(signal));
	}
	return (result * signal);
}
