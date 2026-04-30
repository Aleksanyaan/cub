/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zaleksan <zaleksan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/17 16:12:55 by zaleksan          #+#    #+#             */
/*   Updated: 2025/01/24 13:58:02 by zaleksan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_calloc(size_t count, size_t size)
{
	void	*cmem;
	size_t	total_size;

	total_size = size * count;
	if (count != 0 && total_size / count != size)
		return (0);
	if (total_size == 0)
		total_size = 1;
	cmem = (void *)malloc(total_size);
	if (!cmem)
		return (0);
	ft_bzero(cmem, total_size);
	return (cmem);
}
