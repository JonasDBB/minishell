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

static int		cmp(char c, char prev, char stop)
{
	if (stop == ' ')
	{
		if (c == '\'' || c == '\"' || c == '\t')
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

void			split_at_start(t_list *tmp, t_token *current)
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

void			split_middle(t_list *tmp, t_token *cur, const char *buff, int i)
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

void			setup_split(t_list *tmp, t_token *current)
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
		if (alsocmp(current->str[i], current->str[i - 1]))
		{
			buff[i] = 0;
			split_middle(tmp, current, buff, i);
			break ;
		}
		i++;
	}
	free(buff);
}

static void		find_breaks(t_list **tokenlist)
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

t_list			*get_token_data(char *line, int i, char **buff, char stop)
{
	int		length;
	char	space;
	t_list	*new;

	if (stop == '\'')
		unsetescape(line);
	length = len(line, stop, i);
	if (length == -1)
	{
		free(*buff);
		return (NULL);
	}
	ft_strlcpy(*buff, line + i, length + 1);
	if (stop == ' ')
		space = line[i + length];
	else
		space = line[i + length + 1];
	new = ft_lstnew(new_token(*buff, stop, space));
	malloc_check(new);
	return (new);
}

int				parse_token(char *line, int i, char **buff, t_list **tokenlist)
{
	char	stop;
	t_list	*new;

	stop = ' ';
	while (ft_isspace(line[i]))
		i++;
	if (!line[i])
		return (-1);
	if (ft_strchr("\'\"", line[i]))
		if (!i || line[i - 1] != escape)
		{
			stop = line[i];
			i++;
		}
	new = get_token_data(line, i, &(*buff), stop);
	if (!new)
	{
		ft_lstclear(&(*tokenlist), free_one_token);
		return (-2);
	}
	ft_lstadd_back(&(*tokenlist), new);
	i += len(line, stop, i);
	if (stop != ' ')
		i++;
	return (i);
}

bool			check_if_input_valid(char *inputline)
{
	int	i;

	i = 0;
	if (!ft_strlen(inputline))
		return (false);
	while (ft_isspace(inputline[i]))
		i++;
	if (!inputline[i])
		return (false);
	return (true);
}

t_list			*tokenizer(char *inputline)
{
	t_list	*tokenlist;
	int		i;
	char	*buff;

	if (!check_if_input_valid(inputline))
		return (NULL);
	i = 0;
	buff = malloc(ft_strlen(inputline + i));
	malloc_check(buff);
	tokenlist = NULL;
	while (inputline[i])
	{
		setescape(inputline);
		i = parse_token(inputline, i, &buff, &tokenlist);
		if (i == -1)
			break ;
		if (i == -2)
			return (NULL);
	}
	free(buff);
	find_breaks(&tokenlist);
	((t_token*)ft_lstlast(tokenlist)->content)->space_after = false;
	return (tokenlist);
}
