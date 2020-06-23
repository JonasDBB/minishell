/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_strlcat.c                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: jbennink <marvin@codam.nl>                   +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/10/28 13:29:42 by jbennink      #+#    #+#                 */
/*   Updated: 2020/06/03 14:14:03 by jbennink      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <stddef.h>
#include "libft.h"

size_t	ft_strlcat(char *dst, const char *src, size_t dstsize)
{
	size_t i;
	size_t j;

	i = 0;
	j = ft_strlen(src);
	while (*dst && dstsize > 0)
	{
		dst++;
		dstsize--;
		i++;
	}
	while (dstsize > 1 && *src)
	{
		*dst = *src;
		dstsize--;
		src++;
		dst++;
	}
	if (dstsize != 0)
		*dst = '\0';
	return (i + j);
}
