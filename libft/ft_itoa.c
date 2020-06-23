/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_itoa.c                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: jbennink <jbennink@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/10/30 12:26:35 by jbennink      #+#    #+#                 */
/*   Updated: 2020/06/03 14:15:12 by jbennink      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "libft.h"

static int	digits(long n)
{
	int	i;

	i = 0;
	if (n < 0)
	{
		i++;
		n = -n;
	}
	while (n)
	{
		i++;
		n /= 10;
	}
	return (i);
}

char		*ft_itoa(int n)
{
	char	*result;
	int		i;
	long	n2;

	n2 = n;
	if (n2 == 0 || n2 == -0)
		return (ft_strdup("0"));
	i = digits(n2);
	result = malloc(sizeof(char) * i + 1);
	if (!result)
		return (NULL);
	result[i] = '\0';
	i--;
	if (n2 < 0)
	{
		result[0] = '-';
		n2 = -n2;
	}
	while (n2)
	{
		result[i] = n2 % 10 + '0';
		n2 /= 10;
		i--;
	}
	return (result);
}
