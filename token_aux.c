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

#include "minishell.h"

void	merge_tokens(t_list *current, t_list *next_elem)
{
	t_token	*current_token;
	t_token	*next_token;
	char	*newstr;

	current_token = (t_token*)current->content;
	next_token = (t_token*)next_elem->content;
	current_token->space_after = next_token->space_after;
	newstr = ft_strjoin(current_token->str, next_token->str);
	malloc_check(newstr);
	free(current_token->str);
	current_token->str = ft_strdup(newstr);
	malloc_check(current_token->str);
	free(newstr);
	current->next = current->next->next;
	free_one_token(next_elem->content);
	free(next_elem);
}

void	create_append(t_list *tokenlist)
{
	t_list	*tmp;
	char	first;
	char	second;

	tmp = tokenlist;
	while (tmp && tmp->next)
	{
		first = ((t_token*)tmp->content)->str[0];
		second = ((t_token*)tmp->next->content)->str[0];
		if (first == '>' && second == '>'
			&& !((t_token*)tmp->content)->space_after)
			merge_tokens(tmp, tmp->next);
		tmp = tmp->next;
	}
}

void	setescape(char *s)
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

void	unsetescape(char *s)
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

void	unsetescape_if_literal(char *s)
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
