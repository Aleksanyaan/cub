/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zaleksan <zaleksan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/24 13:30:50 by zaleksan          #+#    #+#             */
/*   Updated: 2025/01/27 20:38:45 by zaleksan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	word_count(char const *s, char c)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	while (s[i] == c)
		i++;
	while (s[i] != '\0')
	{
		if (s[i] != c && (s[i + 1] == c || s[i + 1] == '\0'))
			count++;
		i++;
	}
	return (count);
}

static char	*fill_word(char const *s, int len)
{
	char	*word;
	int		i;

	i = 0;
	word = (char *)malloc((len + 1) * sizeof(char));
	if (!word)
		return (0);
	while (i < len)
	{
		word[i] = s[i];
		i++;
	}
	word[i] = '\0';
	return (word);
}

static void	*ft_free(char **res, int size)
{
	int	i;

	i = 0;
	while (i < size)
	{
		free(res[i]);
		i++;
	}
	free(res);
	return (NULL);
}

static char	**split_copy(const char *s, char c, char **res)
{
	int	word_len_val;
	int	i;

	i = 0;
	while (*s != '\0')
	{
		while (*s == c && *s != '\0')
			s++;
		word_len_val = 0;
		while (s[word_len_val] != '\0' && s[word_len_val] != c)
			word_len_val++;
		if (word_len_val != 0)
			res[i] = fill_word(s, word_len_val);
		else
			break ;
		if (!res[i])
			ft_free(res, i + 1);
		s += word_len_val;
		i++;
	}
	res[i] = NULL;
	return (res);
}

char	**ft_split(char const *s, char c)
{
	char	**res;
	int		size;

	if (!s)
		return (NULL);
	size = word_count(s, c);
	res = (char **)malloc((size + 1) * sizeof(char *));
	if (!res)
		return (NULL);
	return (split_copy(s, c, res));
}
