/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_memccpy.c                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: jbennink <marvin@codam.nl>                   +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/10/28 13:28:26 by jbennink      #+#    #+#                 */
/*   Updated: 2020/06/03 14:14:43 by jbennink      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <stddef.h>

void	*ft_memccpy(void *dst, const void *src, int c, size_t n)
{
	size_t			i;
	unsigned char	*srcstr;
	unsigned char	*dststr;

	srcstr = (unsigned char*)src;
	dststr = (unsigned char*)dst;
	i = 0;
	while (i < n)
	{
		*dststr = *srcstr;
		if (*srcstr == (unsigned char)c)
			return (dststr + 1);
		dststr++;
		srcstr++;
		i++;
	}
	return (NULL);
}
