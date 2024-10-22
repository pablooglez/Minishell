/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcpy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pabloglez <pabloglez@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/07 13:38:50 by pablogon          #+#    #+#             */
/*   Updated: 2024/10/22 20:20:22 by pabloglez        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcpy(char *dest, const char *src, size_t n)
{
	size_t	i;

	i = 0;
	while (src && *(src + i))
		i++;
	if (!n)
		return (i);
	while (--n && src && *src)
		*dest++ = *src++;
	*dest = '\0';
	return (i);
}
