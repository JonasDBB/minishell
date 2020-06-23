/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_strrchr.c                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: jbennink <marvin@codam.nl>                   +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/10/28 13:30:13 by jbennink      #+#    #+#                 */
/*   Updated: 2020/06/03 14:13:45 by jbennink      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <stddef.h>

char	*ft_strrchr(const char *s, int c)
{
	char	*str;

	str = (char*)s;
	while (*str)
		str++;
	if (c == 0 && *str == 0)
		return (char*)(str);
	while (*str != *s)
	{
		if (*str == c)
			return (char*)(str);
		str--;
	}
	if (*str == c)
		return (char*)(str);
	return (NULL);
}
