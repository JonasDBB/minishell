/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   split_tokens.c                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: jbennink <jbennink@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/10/25 15:10:05 by jbennink      #+#    #+#                 */
/*   Updated: 2020/10/25 15:10:05 by jbennink      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../incl/minishell.h"

static void	split_at_start(t_list *tmp, t_token *current)
{
	t_list	*new;
	char	s[2];

	s[1] = 0;
	new = ft_lstnew(new_token((current->str + 1), ' ', 'a'));
	malloc_check(new);
	if (current->space_after)
		((t_token*)new->content)->space_after = true;
	s[0] = current->str[0];
	free(current->str);
	current->str = ft_strdup(s);
	malloc_check(current->str);
	current->space_after = false;
	new->next = tmp->next;
	new->previous = tmp;
	tmp->next = new;
}

static void	split_middle(t_list *tmp, t_token *cur, const char *buff, int i)
{
	t_list	*new;
	char	*new_buff;
	int		j;

	new_buff = malloc(ft_strlen(cur->str) + i);
	malloc_check(new_buff);
	j = 0;
	while (cur->str[i])
	{
		new_buff[j] = cur->str[i];
		i++;
		j++;
	}
	new_buff[j] = 0;
	free(cur->str);
	cur->str = ft_strdup(buff);
	malloc_check(cur->str);
	new = ft_lstnew(new_token((new_buff), ' ', ' '));
	if (!cur->space_after)
		((t_token*)new->content)->space_after = false;
	free(new_buff);
	cur->space_after = false;
	new->next = tmp->next;
	new->previous = tmp;
	tmp->next = new;
}

static void	setup_split(t_list *tmp, t_token *current)
{
	char	*buff;
	int		i;

	buff = malloc(ft_strlen(current->str));
	malloc_check(buff);
	i = 1;
	while (current->end == ' ' && current->str[i])
	{
		buff[0] = current->str[0];
		buff[i] = current->str[i];
		if (is_splitting_char(current->str[i], current->str[i - 1]))
		{
			buff[i] = 0;
			split_middle(tmp, current, buff, i);
			break ;
		}
		i++;
	}
	free(buff);
}

void		find_breaks(t_list **tokenlist)
{
	t_list	*tmp;
	t_token	*curr;

	tmp = *tokenlist;
	while (tmp)
	{
		curr = (t_token*)tmp->content;
		if (curr->end == ' ' && ft_strchr(";|<>", curr->str[0]) && curr->str[1])
		{
			split_at_start(tmp, curr);
			continue ;
		}
		setup_split(tmp, curr);
		tmp = tmp->next;
	}
}
