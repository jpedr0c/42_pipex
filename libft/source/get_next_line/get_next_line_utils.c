/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jocardos <jocardos@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/24 09:55:53 by jocardos          #+#    #+#             */
/*   Updated: 2022/05/25 13:59:47 by jocardos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

size_t	ft_strlen(char *s)
{
	size_t	i;

	i = 0;
	if (!s)
		return (0);
	while (s[i])
		i++;
	return (i);
}

char	*ft_strchr(char *s, int c)
{
	size_t	i;

	i = 0;
	if (!s)
		return (0);
	while (s[i])
	{
		if (s[i] == (unsigned char )c)
			return (s + i);
		i++;
	}
	if (c == '\0')
		return (s + i);
	return (0);
}

char	*ft_strjoin(char *aux_str, char *buffer)
{
	char	*str;
	size_t	i1;
	size_t	i2;

	if (!aux_str)
	{
		aux_str = malloc(1 * sizeof(char));
		aux_str[0] = '\0';
	}
	if (!aux_str || !buffer)
		return (NULL);
	str = malloc(sizeof(char) * (ft_strlen(aux_str) + ft_strlen(buffer) + 1));
	if (!str)
		return (NULL);
	i1 = -1;
	if (aux_str)
		while (aux_str[++i1])
			str[i1] = aux_str[i1];
	i2 = 0;
	while (buffer[i2])
		str[i1++] = buffer[i2++];
	str[ft_strlen(aux_str) + ft_strlen(buffer)] = '\0';
	free(aux_str);
	return (str);
}

char	*return_line(char *aux_str)
{
	size_t	i;
	char	*str;

	i = 0;
	if (!aux_str[i])
		return (NULL);
	while (aux_str[i] && aux_str[i] != '\n')
		i++;
	str = malloc(sizeof(char) * (i + 2));
	if (!str)
		return (NULL);
	i = 0;
	while (aux_str[i] && aux_str[i] != '\n')
	{
		str[i] = aux_str[i];
		i++;
	}
	if (aux_str[i] == '\n')
	{
		str[i] = aux_str[i];
		i++;
	}
	str[i] = '\0';
	return (str);
}

char	*new_aux_str(char *aux_str)
{
	size_t	i;
	size_t	a;
	char	*str;

	a = 0;
	while (aux_str[a] && aux_str[a] != '\n')
		a++;
	if (!aux_str[a])
	{
		free(aux_str);
		return (NULL);
	}
	str = malloc(sizeof(char) * (ft_strlen(aux_str) - a + 1));
	if (!str)
		return (NULL);
	i = 0;
	a++;
	while (aux_str[a])
		str[i++] = aux_str[a++];
	str[i] = '\0';
	free(aux_str);
	return (str);
}
