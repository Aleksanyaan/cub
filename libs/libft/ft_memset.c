/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zaleksan <zaleksan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/16 15:04:42 by zaleksan          #+#    #+#             */
/*   Updated: 2025/01/16 15:29:31 by zaleksan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memset(void *b, int c, size_t len)
{
	unsigned int	i;
	unsigned char	*temp_c;
	unsigned char	*temp_b;

	i = 0;
	temp_b = b;
	temp_c = temp_b;
	while (i < len)
	{
		temp_c[i] = c;
		i++;
	}
	return (b);
}
