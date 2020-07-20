/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_replace.c                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: jbennink <jbennink@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/07/20 13:52:29 by jbennink      #+#    #+#                 */
/*   Updated: 2020/07/20 14:37:15 by jbennink      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

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

int		getnext(char *source, char *old)
{
	int	i;
	int	j;

	i = 0;
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
				return (i);
		}
		i -= (j - 1);
		j -= j;
	}
	return (-1);
}

char	*ft_replace(char *source, char *old, char *new)
{
	int		len;
	int		count;
	char	*result;
	int		start_of_next;
	int		it_source;
	int		it_result;
	int		len_new;
	int		len_old;
	int		i;

	len_new = strlen(new);
	len_old = strlen(old);
	len = strlen(source);
	count = countwords(source, old);
	len += count * (len_new - len_old);
	// printf("count = %i, len = %i\n", count, len);
	result = malloc(len + 1);
	if (!result)
		return(NULL);
	start_of_next = 0;
	it_source = 0;
	it_result = 0;
	while (source[it_source])
	{
		start_of_next = getnext(source + it_source, old);
		if (start_of_next < 0)
			break ;
		while (it_source < start_of_next)
		{
			result[it_result] = source[it_source];
			it_result++;
			it_source++;
		}
		i = 0;
		while (i < len_new)
		{
			result[it_result] = new[i];
			it_result++;
			i++;
		}
		it_source += len_old;
	}
	result[it_result] = 0;
	return (result);
}

int		main(void)
{
	char *s1;
	char *s2;

	s1 = strdup("abcdefgabcdefgabcdefgabcdefg abcdefg abcdefg");
	s2 = ft_replace(s1, "cde", "12");
	printf("after:\n%s\n", s2);
}