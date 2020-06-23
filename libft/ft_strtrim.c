/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_strtrim.c                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: jbennink <jbennink@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/10/30 14:21:42 by jbennink      #+#    #+#                 */
/*   Updated: 2020/06/03 14:13:42 by jbennink      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "libft.h"

static int	ft_charcmp(char a, char const *set)
{
	int	i;

	i = 0;
	while (set[i])
	{
		if (a == set[i])
			return (1);
		i++;
	}
	return (0);
}

static	int	getmallen(char const *s1, char const *set)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (s1[j])
		j++;
	while (ft_charcmp(s1[i], set) == 1)
		i++;
	j--;
	while (ft_charcmp(s1[j], set) == 1)
		j--;
	if ((j - i) <= 0)
		return (0);
	return (j - i);
}

char		*ft_strtrim(char const *s1, char const *set)
{
	char	*result;
	int		i;
	int		len;

	if (!s1 || !set)
		return (NULL);
	len = getmallen(s1, set) + 1;
	result = malloc(len + 1);
	if (!result)
		return (NULL);
	while (ft_charcmp(*s1, set) == 1)
		s1++;
	i = 0;
	while (*s1 && i < len)
	{
		result[i] = *s1;
		i++;
		s1++;
	}
	result[i] = '\0';
	return (result);
}
