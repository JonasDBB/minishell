/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   echo0.c                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: jbennink <jbennink@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/07/08 11:35:43 by jbennink      #+#    #+#                 */
/*   Updated: 2020/07/20 13:49:40 by jbennink      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_shit	new_element(void)
{
	t_shit	result;

	result.start = 0;
	result.len = 0;
	result.literal = false;
	result.space = false;
	return (result);
}

int		count_elems(char *s)
{
	int		result;
	int		i;
	char	stop;
	char	quotes[2];

	quotes[0] = '\'';
	quotes[1] = '\"';
	result = 0;
	i = 0;
	stop = ' ';
	while (s[i])
	{
		while (s[i] == ' ' && s[i])
			i++;
		if (!s[i])
			return (result);
		if (s[i] == '\"')
			stop = '\"';
		if (s[i] == '\'')
			stop = '\'';
		if (stop != ' ')
			i++;
		while (s[i] != stop && s[i] && !ft_strchr(quotes, s[i]))
			i++;
		if (!s[i] && stop != ' ')
			return (-1);
		if (stop != ' ')
			i++;
		result++;
		if (!s[i])
			break ;
		stop = ' ';
	}
	return (result);
}

int		get_arg_data(char *s, t_shit *arg)
{
	int		i;
	char	stop;
	char	quotes[2];

	quotes[0] = '\'';
	quotes[1] = '\"';
	i = 0;
	stop = ' ';
	while (s[i] == ' ' && s[i])
		i++;
	if (!s[i])
		return (i);
	if (s[i] == '\"')
		stop = '\"';
	if (s[i] == '\'')
	{
		stop = '\'';
		arg->literal = true;
	}
	if (stop != ' ')
		i++;
	arg->start += i;
	while (s[i] != stop && s[i] && !ft_strchr(quotes, s[i]))
	{
		i++;
		arg->len++;
	}
	if (stop != ' ')
		i++;
	if (!s[i])
		return (i);
	if (s[i] == ' ')
		arg->space = true;
	return (i);
}

void	ms_echo(char *s)
{
	int		count;
	t_shit	*args;
	int		i;
	int		j;
	char	newline;

	count = count_elems(s);
	if (count == -1)
	{
		write(1, "Error: missing closing quote on argument\n", 41);
		return ;
	}
	args = malloc(sizeof(t_shit) * count);
	i = 0;
	j = 0;
	while (i < count)
	{
		args[i] = new_element();
		args[i].start = j;
		j += get_arg_data(s + j, &args[i]);
		args[i].content = ft_substr(s, args[i].start, args[i].len);
		i++;
	}
	i = 0;
	newline = true;
	if (!ft_strcmp(args[0].content, "-n"))
	{
		newline = false;
		i++;
		free(args[0].content);
	}
	while (i < count)
	{
		write(1, args[i].content, args[i].len);
		if (args[i].space)
			write(1, " ", 1);
		free(args[i].content);
		i++;
	}
	if (newline)
		write(1, "\n", 1);
	free(args);
}
