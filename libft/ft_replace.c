/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_replace.c                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: jbennink <jbennink@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/07/20 13:52:29 by jbennink      #+#    #+#                 */
/*   Updated: 2020/08/07 15:11:37 by jbennink      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "libft.h"

int		countwords(char *source, char *old)
{
	int	i;
	int	j;
	int	result;

	i = 0;
	j = 0;
	result = 0;
	while (source[i])
	{
		while (source[i] != old[j] && source[i])
			i++;
		while (source[i] == old[j] && source[i] && old[j])
		{
			i++;
			j++;
			if (!old[j])
				result++;
		}
		i -= (j - 1);
		j -= j;
	}
	return (result);
}

int		getnext(char *source, char *old, int i)
{
	int	j;

	j = 0;
	while (source[i])
	{
		while (source[i] != old[j] && source[i])
			i++;
		while (source[i] == old[j] && source[i] && old[j])
		{
			i++;
			j++;
			if (!old[j])
				return (i - j);
		}
		i -= (j - 1);
		j -= j;
	}
	return (-1);
}

void	fill_result(t_garbo *garbo, char *source, char *new, char *old)
{
	garbo->start_of_next = getnext(source, old, garbo->it_source);
	if (garbo->start_of_next < 0)
	{
		while (source[garbo->it_source])
		{
			garbo->result[garbo->it_result] = source[garbo->it_source];
			garbo->it_result++;
			garbo->it_source++;
		}
		return ;
	}
	while (garbo->it_source < garbo->start_of_next)
	{
		garbo->result[garbo->it_result] = source[garbo->it_source];
		garbo->it_result++;
		garbo->it_source++;
	}
	garbo->it_new = 0;
	while (garbo->it_new < garbo->len_new)
	{
		garbo->result[garbo->it_result] = new[garbo->it_new];
		garbo->it_result++;
		garbo->it_new++;
	}
	garbo->it_source += garbo->len_old;
}

char	*ft_replace(char *source, char *old, char *new)
{
	t_garbo	garbo;

	garbo.len_new = ft_strlen(new);
	garbo.len_old = ft_strlen(old);
	garbo.len = ft_strlen(source);
	garbo.count = countwords(source, old);
	garbo.len += garbo.count * (garbo.len_new - garbo.len_old);
	garbo.result = malloc(garbo.len + 1);
	if (!garbo.result)
		return (NULL);
	garbo.start_of_next = 0;
	garbo.it_source = 0;
	garbo.it_result = 0;
	while (source[garbo.it_source])
		fill_result(&garbo, source, new, old);
	free(source);
	garbo.result[garbo.it_result] = 0;
	return (garbo.result);
}
/*
** free(source) 4 lines up, only if you want to get rid of source!!!
*/
