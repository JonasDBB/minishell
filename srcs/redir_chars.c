/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   redir_chars.c                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: jbennink <jbennink@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/10/25 15:08:53 by jbennink      #+#    #+#                 */
/*   Updated: 2020/10/25 15:08:53 by jbennink      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../incl/minishell.h"

void	set_redirects(char *str)
{
	if (str[0] == '<')
		str[0] = redirect_input;
	if (str[0] == '>' && !str[1])
		str[0] = redirect_trunc;
	if (str[0] == '>' && str[1] == '>')
	{
		str[0] = redirect_append;
		str[1] = 0;
	}
}

void	set_redirs(t_list *tokenlist)
{
	t_list	*tmp;
	t_token	*current;

	tmp = tokenlist->next;
	while (tmp)
	{
		current = (t_token*)tmp->content;
		if (current->end == ' ')
			set_redirects(current->str);
		tmp = tmp->next;
	}
}

bool	is_redir(char c)
{
	if (c == redirect_input)
		return (true);
	if (c == redirect_append)
		return (true);
	if (c == redirect_trunc)
		return (true);
	return (false);
}

void	create_append(t_list *tokenlist)
{
	t_list	*tmp;
	char	first;
	char	second;

	tmp = tokenlist->next;
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
