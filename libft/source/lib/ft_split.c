/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jocardos <jocardos@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/18 14:21:53 by jocardos          #+#    #+#             */
/*   Updated: 2022/08/18 11:08:22 by jocardos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/libft.h"

static int	count_words(const char *str, char c)
{
	int	number_words;
	int	is_word;

	number_words = 0;
	is_word = 0;
	while (*str)
	{
		if (*str != c && is_word == 0)
		{
			is_word = 1;
			number_words++;
		}
		else if (*str == c)
			is_word = 0;
		str++;
	}
	return (number_words);
}

static char	*duplicate_word(const char *str, int init, int end)
{
	char	*word;
	int		i;

	i = 0;
	word = malloc((end - init + 1) * sizeof(char));
	while (init < end)
		word[i++] = str[init++];
	word[i] = '\0';
	return (word);
}

char	**ft_split(char const *s, char c)
{
	size_t	i;
	size_t	j;
	int		index;
	char	**split;

	split = malloc((count_words(s, c) + 1) * sizeof(char *));
	if (!s || !split)
		return (0);
	i = 0;
	j = 0;
	index = -1;
	while (i <= ft_strlen(s))
	{
		if (s[i] != c && index < 0)
			index = i;
		else if ((s[i] == c || i == ft_strlen(s)) && index >= 0)
		{
			split[j++] = duplicate_word(s, index, i);
			index = -1;
		}
		i++;
	}
	split[j] = 0;
	return (split);
}
