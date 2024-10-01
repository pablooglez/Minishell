/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pablogon <pablogon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/13 20:33:15 by pablogon          #+#    #+#             */
/*   Updated: 2024/08/24 18:23:30 by pablogon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_substr(const char *s, unsigned int start, size_t len)
{
	char	*str;
	size_t	m_len;

	if (start > ft_strlen(s))
		m_len = 0;
	else
		m_len = ft_strlen(s) - start;
	if (len > m_len)
		len = m_len;
	len += 1;
	str = (char *)malloc(len);
	if (str == NULL)
		return (NULL);
	if (len > 1)
		ft_strlcpy(str, s + start, len);
	else
		str[0] = '\0';
	return (str);
}
