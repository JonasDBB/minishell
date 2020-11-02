/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   token_helper.c                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: jbennink <jbennink@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/10/25 14:53:15 by jbennink      #+#    #+#                 */
/*   Updated: 2020/10/25 14:53:15 by jbennink      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../incl/minishell.h"

t_token			*new_token(char *content, char end, char space_after)
{
	t_token *ret;

	if (end == '\'')
		unsetescape(content);
	ret = malloc(sizeof(t_token));
	malloc_check(ret);
	ret->str = ft_strdup(content);
	malloc_check(ret->str);
	if (end == '\'')
		ret->literal = true;
	else
		ret->literal = false;
	if (ft_isspace(space_after))
		ret->space_after = true;
	else
		ret->space_after = false;
	ret->end = end;
	return (ret);
}

static t_list	*split_current(t_list *tmp, const t_token *current)
{
	int		i;
	t_list	*tmp2;
	t_token	*new;
	char	**buff;

	buff = ft_split(current->str, ' ');
	malloc_check(buff);
	i = 0;
	while (buff[i])
	{
		new = new_token(buff[i], ' ', ' ');
		malloc_check(new);
		lst_add_before(tmp, new);
		i++;
	}
	tmp2 = tmp->previous;
	free_array(buff);
	lst_remove_current(tmp, free_one_token);
	tmp = tmp2;
	return (tmp);
}

static bool		need_split(char *arg)
{
	int	i;

	i = 0;
	while (arg[i])
	{
		if (ft_isspace(arg[i]))
			return (true);
		i++;
	}
	return (false);
}

void			retokenize_expanded_vars(t_list *tokenlist)
{
	t_list	*tmp;
	t_token	*current;

	tmp = tokenlist;
	while (tmp)
	{
		current = (t_token*)tmp->content;
		if (current->end == ' ' && need_split(current->str))
		{
			tmp = split_current(tmp, current);
		}
		tmp = tmp->next;
	}
}

void			merge_tokens(t_list *current, t_list *next_elem)
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
