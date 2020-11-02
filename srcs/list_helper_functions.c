/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   list_helper_functions.c                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: jbennink <jbennink@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/10/25 14:58:32 by jbennink      #+#    #+#                 */
/*   Updated: 2020/10/25 14:58:32 by jbennink      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../incl/minishell.h"

static void	lol(void)
{
}

void		lst_remove_current(t_list *current, void (*del)(void*))
{
	if (!current->next && !current->previous)
		lol();
	else if (!current->previous)
		current->next->previous = NULL;
	else if (!current->next)
		current->previous->next = NULL;
	else
	{
		current->previous->next = current->next;
		current->next->previous = current->previous;
	}
	del(current->content);
	free(current);
}

void		lst_add_before(t_list *current, void *content)
{
	t_list	*new;

	new = ft_lstnew(content);
	malloc_check(new);
	if (!current->previous)
	{
		new->next = current;
		current->previous = new;
	}
	else
	{
		new->previous = current->previous;
		new->next = current;
		current->previous->next = new;
		current->previous = new;
	}
}

void		concat_list(t_list *tokenlist)
{
	t_list	*current;
	t_list	*next_elem;
	t_token	*current_token;
	t_token	*next_token;

	current = tokenlist;
	while (current)
	{
		current_token = (t_token*)current->content;
		if (!current_token->space_after && current->next)
		{
			next_elem = current->next;
			next_token = (t_token*)next_elem->content;
			if (is_redir(current_token->str[0]) || (next_token->end == ' ' &&
				(ft_strchr(";|<>", current_token->str[0])
				|| ft_strchr(";|<>", next_token->str[0]))))
			{
				current = current->next;
				continue ;
			}
			merge_tokens(current, next_elem);
		}
		else
			current = current->next;
	}
}
