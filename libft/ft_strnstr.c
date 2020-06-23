/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_strnstr.c                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: jbennink <marvin@codam.nl>                   +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/10/28 13:30:20 by jbennink      #+#    #+#                 */
/*   Updated: 2020/06/03 14:13:48 by jbennink      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <stddef.h>
#include "libft.h"

static int	ft_strcmp(const char *needle, const char *haystack)
{
	int	i;

	i = 0;
	while (needle[i] && haystack[i])
	{
		if (needle[i] != haystack[i])
			return (needle[i] - haystack[i]);
		i++;
	}
	return (0);
}

char		*ft_strnstr(const char *haystack, const char *needle, size_t len)
{
	size_t	i;
	size_t	nlen;

	nlen = ft_strlen(needle);
	if (nlen == 0)
		return (char*)(haystack);
	if (nlen > ft_strlen(haystack) || len == 0 || ft_strlen(haystack) == 0)
		return (NULL);
	i = 0;
	while (i < (len - nlen + 1) && *haystack)
	{
		if (*haystack == needle[0])
		{
			if (ft_strcmp(needle, haystack) == 0)
				return (char*)(haystack);
		}
		i++;
		haystack++;
	}
	return (NULL);
}
