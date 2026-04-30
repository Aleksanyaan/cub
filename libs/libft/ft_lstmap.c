/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstmap.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zaleksan <zaleksan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/26 20:21:44 by zaleksan          #+#    #+#             */
/*   Updated: 2025/01/26 20:44:32 by zaleksan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list	*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *))
{
	t_list	*newlst;
	t_list	*newobj;

	if (!lst || !del || !f)
		return (NULL);
	newlst = NULL;
	while (lst)
	{
		newobj = ft_lstnew(f(lst->content));
		if (!newobj)
		{
			ft_lstclear(&newlst, del);
			return (NULL);
		}
		ft_lstadd_back(&newlst, newobj);
		lst = lst->next;
	}
	return (newlst);
}
