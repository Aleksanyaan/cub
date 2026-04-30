/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zaleksan <zaleksan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/21 16:19:30 by zaleksan          #+#    #+#             */
/*   Updated: 2025/01/31 13:59:32 by zaleksan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	number_length(int nb)
{
	int	len;

	len = 0;
	if (nb <= 0)
		len = 1;
	while (nb != 0)
	{
		nb /= 10;
		len++;
	}
	return (len);
}

char	*transform(char *res, int n, int len)
{
	while (n > 0)
	{
		res[--len] = (n % 10) + '0';
		n /= 10;
	}
	return (res);
}

char	*ft_itoa(int n)
{
	char	*res;
	int		len;
	int		sign;

	sign = 0;
	if (n == -2147483648)
		return (ft_strdup("-2147483648"));
	if (n < 0)
	{
		sign = 1;
		n = -n;
	}
	len = number_length(n) + sign;
	res = (char *)malloc(len + 1);
	if (!res)
		return (0);
	res[len] = '\0';
	if (sign)
		res[0] = '-';
	transform(res, n, len);
	if (n == 0 && !sign)
		res[0] = '0';
	return (res);
}
