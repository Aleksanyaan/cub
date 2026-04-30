/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zaleksan <zaleksan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/18 20:18:00 by zaleksan          #+#    #+#             */
/*   Updated: 2025/01/24 16:55:18 by zaleksan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strtrim(char const *s1, char const *set)
{
	char	*res;
	int		i;
	int		start;
	int		end;

	i = 0;
	start = 0;
	end = ft_strlen(s1) - 1;
	if (!s1 || !set)
		return (0);
	while (s1[start] && ft_strchr(set, s1[start]))
		start++;
	while (end > start && ft_strchr(set, s1[end]))
		end--;
	res = (char *)malloc(end - start + 2);
	if (!res)
		return (0);
	i = 0;
	while (start <= end)
		res[i++] = s1[start++];
	res[i] = '\0';
	return (res);
}
