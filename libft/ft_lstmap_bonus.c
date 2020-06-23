/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_lstmap_bonus.c                                  :+:    :+:            */
/*                                                     +:+                    */
/*   By: jbennink <jbennink@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/10/31 17:27:03 by jbennink      #+#    #+#                 */
/*   Updated: 2020/06/03 14:14:52 by jbennink      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdlib.h>

t_list	*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *))
{
	t_list	*start;
	t_list	*new;
	t_list	*prev;

	if (!lst)
		return (NULL);
	new = ft_lstnew(f(lst->content));
	start = new;
	lst = lst->next;
	prev = start;
	while (lst)
	{
		new = ft_lstnew(f(lst->content));
		if (!new)
		{
			ft_lstclear(&start, del);
			return (NULL);
		}
		prev->next = new;
		prev = new;
		lst = lst->next;
	}
	return (start);
}
