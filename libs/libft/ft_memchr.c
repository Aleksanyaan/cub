/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zaleksan <zaleksan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/17 11:13:51 by zaleksan          #+#    #+#             */
/*   Updated: 2025/01/21 15:11:51 by zaleksan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memchr(const void *s, int c, size_t n)
{
	unsigned char	*ucs;
	unsigned char	ucc;
	size_t			i;

	ucs = (unsigned char *)s;
	ucc = (unsigned char)c;
	i = 0;
	while (i < n)
	{
		if (ucs[i] == ucc)
			return (&ucs[i]);
		i++;
	}
	return (0);
}
