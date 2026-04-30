/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zaleksan <zaleksan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/23 20:08:42 by zaleksan          #+#    #+#             */
/*   Updated: 2025/01/23 20:26:57 by zaleksan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_putnbr_fd(int n, int fd)
{
	unsigned int		nbr;
	char				res;

	if (n == -2147483648)
		write(fd, "-2147483648", 11);
	else
	{
		if (n < 0)
		{
			write (fd, "-", 1);
			nbr = n * -1;
		}
		else
			nbr = n;
		if (nbr / 10 != 0)
			ft_putnbr_fd(nbr / 10, fd);
		res = nbr % 10 + '0';
		write (fd, &res, 1);
	}
}
