/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstadd_back_bonus.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pablogon <pablogon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/20 21:55:39 by pablogon          #+#    #+#             */
/*   Updated: 2023/12/20 22:14:18 by pablogon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstadd_back(t_list **lst, t_list *new)
{
	t_list	*lst_final;

	if (!*lst)
	{
		*lst = new;
		return ;
	}
	lst_final = *lst;
	while ((*lst)->next)
		*(lst) = (*lst)->next;
	(*lst)->next = new;
	*lst = lst_final;
}
