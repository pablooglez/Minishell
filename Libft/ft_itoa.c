/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pablogon <pablogon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/18 19:46:30 by pablogon          #+#    #+#             */
/*   Updated: 2023/12/19 18:08:26 by pablogon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "libft.h"

int	count_number(int n)
{
	int	i;

	i = 0;
	if (n == 0)
		return (1);
	if (n < 0)
	{
		if (n == -2147483648)
		{
			n /= 10;
			i++;
		}
		n = -n;
		i++;
	}
	while (n > 0)
	{
		n /= 10;
		i++;
	}
	return (i);
}

void	ft_setchr(char *res, int n, int i)
{
	while (n > 0)
	{
		res[i] = (n % 10) + '0';
		n /= 10;
		i--;
	}
}

char	*ft_itoa(int n)
{
	char	*res;
	int		i;

	i = count_number(n);
	res = (char *)malloc((count_number(n) + 1) * sizeof(char));
	if (!res)
		return (0);
	res[i] = '\0';
	i--;
	if (n == 0)
		res[0] = '0';
	if (n < 0)
	{
		res[0] = '-';
		if (n == -2147483648)
		{
			res[i] = '8';
			n /= 10;
			i--;
		}
		n = -n;
	}
	ft_setchr(res, n, i);
	return (res);
}
