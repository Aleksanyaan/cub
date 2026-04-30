/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zaleksan <zaleksan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/17 13:17:58 by zaleksan          #+#    #+#             */
/*   Updated: 2025/01/29 13:14:05 by zaleksan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memmove(void *dst, const void *src, size_t len)
{
	size_t	i;
	char	*srcptr;
	char	*dstptr;

	if (!dst && !src)
		return (NULL);
	i = 0;
	srcptr = (char *)src;
	dstptr = (char *)dst;
	if (dstptr > srcptr)
		while (len-- > 0)
			dstptr[len] = srcptr[len];
	else
	{
		while (i < len)
		{
			dstptr[i] = srcptr[i];
			i++;
		}
	}
	return (dst);
}
