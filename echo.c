/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   echo.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: jbennink <jbennink@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/06/24 11:44:18 by jbennink      #+#    #+#                 */
/*   Updated: 2020/06/29 14:06:08 by jbennink      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <stdio.h> //printf include

void	*ft_realloc(void *ptr, size_t size_old, size_t size_new)
{
	void	*result;

	if (!size_new)
		free(ptr);
	result = malloc(size_new);
	if (!result)
		return (NULL);
	ft_memcpy(result, ptr, size_old < size_new ? size_old : size_new);
	if (!size_new)
		free(ptr);
	return (result);
}

int		getlen(char *s, char stop)
{
	int	i;

	i = 0;
	if (stop != ' ')
		i++;
	while (s[i] != stop && s[i])
		i++;
	if (stop != ' ')
		i++;
	return (i);
}

t_arg	new_arg(char *s, char stop, int len)
{
	t_arg	result;

	result.content = malloc(len + 1);
	// if (stop != ' ')
	// 	ft_memcpy(result.content, s + 1, len - 2);
	// else
	ft_memcpy(result.content, s, len);
	result.content[len] = 0;
	result.literal = false;
	if (stop == '\'')
		result.literal = true;
	result.space_after = true;
	result.end = false;
	return (result);
}

t_arg	*split_args(char *s)
{
	t_arg	*result;
	int		i;
	int		j;
	int		len;
	char	stop;

	i = 0;
	j = 0;
	stop = ' ';
	result = malloc(sizeof(t_arg));
	if (!result)
		write(1, "malloc error\n", 13);
	while (s[i])
	{
		while (s[i] == ' ' && s[i])
			i++;
		if (!s[i])
			break ;
		if (s[i] == '\"' || s[i] == '\'')
			stop = s[i];
		len = getlen(s + i, stop);
		if (j)
			result = ft_realloc(result, sizeof(t_arg) * (j + 1), sizeof(t_arg) * (j + 2));
		result[j] = new_arg(s + i, stop, len);
		j++;
		i += len;
		if (!s[i])
			break ;
		if (stop != ' ' && s[i + 1] == stop)
			result[j].space_after = false;
		stop = ' ';
	}
	result[j].space_after = false;
	result[j].end = true;
	return (result);
}

void	ms_echo(char *s)
{
	char	newline_flag;
	int		i;
	t_arg	*words;

	while (*s == ' ')
		s++;
	words = split_args(s);
	newline_flag = 0;
	i = 0;
	if (!ft_strcmp(words[0].content, "-n"))
	{
		newline_flag = 1;
		i++;
	}
	while (1)
	{
		write(1, words[i].content, ft_strlen(words[i].content));
		if (words[i].space_after)
			write(1, " ", 1);
		printf("end is %i\n", (int)words[i].end);
		if (words[i].end)
			break ;
		i++;
	}
	if (newline_flag)
		write(1, "\n", 1);
}
