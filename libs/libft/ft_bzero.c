/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_bzero.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zaleksan <zaleksan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/16 15:32:27 by zaleksan          #+#    #+#             */
/*   Updated: 2026/02/10 20:19:45 by zaleksan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_bzero(void *s, size_t n)
{
	unsigned int	i;
	unsigned char	*temp_s;

	i = 0;
	temp_s = s;
	while (i < n)
	{
		temp_s[i] = 0;
		i++;
	}
}
