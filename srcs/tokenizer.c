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

#include "../incl/minishell.h"

static t_list	*get_token_data(char *line, int i, char **buff, char stop)
{
	int		length;
	char	space;
	t_list	*new;

	if (stop == '\'')
		unsetescape(line);
	length = get_token_len(line, stop, i);
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

static int		parse_token(char *line, int i, char **buff, t_list **tokenlist)
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
	i += get_token_len(line, stop, i);
	if (stop != ' ')
		i++;
	return (i);
}

static bool		check_if_input_valid(char *inputline)
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
