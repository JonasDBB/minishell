/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   tokenizer.c                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: jbennink <jbennink@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/10/02 15:14:27 by jbennink      #+#    #+#                 */
/*   Updated: 2020/10/02 15:14:27 by jbennink      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <stdio.h>

void			setescape(char *s)
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

void			unsetescape(char *s)
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

static t_tokens	*new_token(char *content, char end, char space_after)
{
	t_tokens *ret;

	ret = malloc(sizeof(t_tokens));
	malloc_check(ret);
	ret->content = ft_strdup(content);
	malloc_check(ret->content);
	if (end == '\'')
		ret->literal = true;
	else
		ret->literal = false;
	if (space_after == ' ')
		ret->space_after = true;
	else
		ret->space_after = false;
	ret->end = end;
	return (ret);
}

static int		cmp(char c, char prev, char stop)
{
	if (stop == ' ')
	{
		if (c == '\'' || c == '\"')
		{
			if (prev != escape)
				return (1);
		}
	}
	if (c == stop)
	{
		if (prev != escape)
			return (1);
	}
	return (0);
}

static int		alsocmp(char c, char prev)
{
	if (ft_strchr(";|<>", c))
		if (prev != escape)
			return (1);
	return (0);
}

static int		len(char *str, char stop, int it)
{
	int	i;
	i = it;
	if (i <= 0)
		i = 1;
	while (!cmp(str[i], str[i - 1], stop) && str[i])
		i++;
	if ((stop != ' ' && !str[i]) || str[ft_strlen(str) - 1] == escape)
	{
		write(2, "multiline commands are not supported\n", 37);
		return (-1);
	}
	return (i - it);
}

void			find_breaks(t_list **tokenlist)
{
	t_list		*tmp;
	t_tokens	*current;
	t_list		*new;
	char		s[2];
	char		*buff;
	char		*buf2;
	int			i;
	int			j;

	tmp = *tokenlist;
	s[1] = 0;
	while (tmp)
	{
		i = 1;
		current = (t_tokens*)tmp->content;
		if (current->end == ' ' && ft_strchr(";|<>", current->content[0]) && current->content[1])
		{
			new = ft_lstnew(new_token((current->content + 1), ' ', ' '));
			s[0] = current->content[0];
			free(current->content);
			current->content = ft_strdup(s);
			current->space_after = false;
			malloc_check(current->content);
			new->next = tmp->next;
			tmp->next = new;
			continue ;
		}
		buff = malloc(ft_strlen(current->content));
		malloc_check(buff);
		while (current->end == ' ' && current->content[i])
		{
			buff[0] = current->content[0];
			buff[i] = current->content[i];
			if (alsocmp(current->content[i], current->content[i - 1]))
			{
				buff[i] = 0;
				buf2 = malloc(ft_strlen(current->content) + i);
				malloc_check(buf2);
				j = 0;
				while (current->content[i])
				{
					buf2[j] = current->content[i];
					i++;
					j++;
				}
				buf2[j] = 0;
				free(current->content);
				current->content = ft_strdup(buff);
				malloc_check(current->content);
				new = ft_lstnew(new_token((buf2), ' ', ' '));
				free(buf2);
				current->space_after = false;
				new->next = tmp->next;
				tmp->next = new;
				break ;
			}
			i++;
		}
		free(buff);
		tmp = tmp->next;
	}
}

t_list			*tokenizer(char *inputline)
{
	t_list	*tokenlist;
	int		i;
	char	*buff;
	char	stop;
	int		length;
	int		size;
	char	space;

	i = 0;
	size = 0;
	buff = malloc(ft_strlen(inputline + i));
	malloc_check(buff);
	tokenlist = NULL;
	setescape(inputline);
	while (inputline[i])
	{
		stop = ' ';
		while (inputline[i] == ' ')
			i++;
		if (!inputline[i])
			break ;
		if (ft_strchr("\'\"", inputline[i]))
		{
			if (!i)
			{
				stop = inputline[i];
				i++;
			}
			else if (inputline[i - 1] != escape)
			{
				stop = inputline[i];
				i++;
			}
		}
		length = len(inputline, stop, i);
		if (length == -1)
		{
			free(buff);
			ft_lstclear(&tokenlist, ft_free_one);
			return (NULL);
		}
		ft_strlcpy(buff, inputline + i, length + 1);
		if (stop == ' ')
			space = inputline[i + length];
		else
			space = inputline[i + length + 1];
		ft_lstadd_back(&tokenlist, ft_lstnew(new_token(buff, stop, space)));
		size++;
		if (ft_lstsize(tokenlist) != size)
			leaks_exit("malloc fail", 1);
		i += len(inputline, stop, i);
		if (stop != ' ')
			i++;
	}
	free(buff);
	find_breaks(&tokenlist);
	((t_tokens*)ft_lstlast(tokenlist)->content)->space_after = false;
	return (tokenlist);
}
