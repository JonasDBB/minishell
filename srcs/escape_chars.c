/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   token_aux.c                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: jbennink <jbennink@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/10/09 09:57:10 by jbennink      #+#    #+#                 */
/*   Updated: 2020/10/09 09:57:10 by jbennink      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../incl/minishell.h"

static void	unsetescape_if_literal(char *s)
{
	int i;

	i = 0;
	while (s[i])
	{
		if (s[i] == escape && s[i + 1] && s[i + 1] != '$' && s[i + 1] != '\\')
			s[i] = '\\';
		i++;
	}
}

void		remove_escapes(t_list **tokenlist)
{
	t_list	*tmp;
	t_list	*tmp2;
	t_token	*current;
	char	old[2];

	tmp = *tokenlist;
	old[0] = escape;
	old[1] = 0;
	while (tmp)
	{
		if (((t_token*)tmp->content)->end == '\"')
			unsetescape_if_literal(((t_token*)tmp->content)->str);
		((t_token*)tmp->content)->str =
				ft_replace(((t_token*)tmp->content)->str, old, "");
		malloc_check(((t_token*)tmp->content)->str);
		tmp2 = tmp->next;
		current = (t_token*)tmp->content;
		if (!ft_strcmp("", current->str) && current->end == ' ')
		{
			if (!tmp->previous->previous)
				*tokenlist = tmp->next;
			lst_remove_current(tmp, free_one_token);
		}
		tmp = tmp2;
	}
}

void		setescape(char *s)
{
	int i;

	i = 1;
	if (s[0] == '\\')
		s[0] = escape;
	while (s[i])
	{
		if (s[i] == '\\' && s[i - 1] != escape)
			s[i] = escape;
		i++;
	}
}

void		unsetescape(char *s)
{
	int i;

	i = 0;
	while (s[i])
	{
		if (s[i] == escape)
			s[i] = '\\';
		i++;
	}
}
