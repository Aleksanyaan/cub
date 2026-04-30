/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zaleksan <zaleksan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/16 15:38:25 by zaleksan          #+#    #+#             */
/*   Updated: 2025/01/16 15:56:38 by zaleksan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memcpy(void *dst, const void *src, size_t n)
{
	unsigned char	*temp_dst;
	unsigned char	*temp_src;
	size_t			i;

	temp_dst = (unsigned char *)dst;
	temp_src = (unsigned char *)src;
	i = 0;
	if (dst == 0 && src == 0)
		return (dst);
	while (i < n)
	{
		temp_dst[i] = temp_src[i];
		i++;
	}
	return (dst);
}
