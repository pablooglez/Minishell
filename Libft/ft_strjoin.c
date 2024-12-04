/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pablogon <pablogon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/13 21:04:58 by pablogon          #+#    #+#             */
/*   Updated: 2024/11/26 21:54:29 by pablogon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strjoin(const char *str1, const char *str2)
{
	char	*ret;
	int		len;

	if (!str1 || !str2)
		return (0);
	len = ft_strlen(str1) + ft_strlen(str2);
	ret = malloc(sizeof(char) * (len + 1));
	if (!ret)
		return (0);
	ft_strlcpy(ret, str1, len + 1);
	ft_strlcat(ret, str2, len + 1);
	return (ret);
}

char	*ft_strjoin2(const char *str1, const char *sep, const char *str2)
{
	char	*ret;
	int		len;

	if (!str1 && !sep && !str2)
		return (NULL);
	len = ft_strlen(str1) + ft_strlen(sep) + ft_strlen(str2);
	ret = malloc(sizeof(char) * (len + 1));
	if (ret && str1)
	{
		ft_strlcpy(ret, str1, len + 1);
		if (sep)
			ft_strlcat(ret, sep, len + 1);
		if (str2)
			ft_strlcat(ret, str2, len + 1);
	}
	else if (ret && sep)
	{
		ft_strlcpy(ret, sep, len + 1);
		if (str2)
			ft_strlcat(ret, str2, len + 1);
	}
	else if (ret && str2)
		ft_strlcpy(ret, str2, len + 1);
	return (ret);
}
