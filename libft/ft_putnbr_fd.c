/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_putnbr_fd.c                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: jbennink <jbennink@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/10/30 12:26:51 by jbennink      #+#    #+#                 */
/*   Updated: 2020/06/03 14:14:20 by jbennink      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>

void	ft_putnbr_fd(int n, int fd)
{
	long	n2;
	char	c;

	n2 = n;
	if (n2 < 0)
	{
		write(fd, "-", 1);
		n2 = -n2;
	}
	if (n2 >= 10)
		ft_putnbr_fd(n2 / 10, fd);
	c = n2 % 10 + '0';
	write(fd, &c, 1);
}
