/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_str.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pargev <pargev@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/27 18:26:20 by pargev            #+#    #+#             */
/*   Updated: 2026/01/28 19:46:02 by pargev           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

int	ft_str_only(const char *s, const char *allowed)
{
	int	i;

	i = 0;
	while (s[i])
	{
		if (ft_strchr(allowed, s[i]) == 0)
			return (0);
		i++;
	}
	return (1);
}

char	*ft_strdup_free(char *s1)
{
	char	*dup;
	size_t	i;

	i = 0;
	dup = (char *)malloc(sizeof(char) * (ft_strlen(s1) + 1));
	if (!dup)
		return (free(s1), NULL);
	while (s1[i] != '\0')
	{
		dup[i] = s1[i];
		i++;
	}
	dup[i] = '\0';
	return (free(s1), dup);
}
