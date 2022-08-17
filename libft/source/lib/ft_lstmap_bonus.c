/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstmap_bonus.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jocardos <jocardos@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/18 11:04:34 by jocardos          #+#    #+#             */
/*   Updated: 2022/05/18 16:46:49 by jocardos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "libft.h"

t_list	*ft_lstmap(t_list *lst, void *(*f)(void*), void (*del)(void*))
{
	t_list	*add_func;
	t_list	*new;

	if (lst && f)
	{
		add_func = ft_lstnew(f(lst->content));
		new = add_func;
		while (lst->next)
		{
			lst = lst->next;
			new->next = ft_lstnew(f(lst->content));
			if (!new->next)
			{
				ft_lstclear(&add_func, del);
				return (NULL);
			}
			new = new->next;
		}
		return (add_func);
	}
	return (NULL);
}
