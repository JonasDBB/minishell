/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_atoi.c                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: jbennink <marvin@codam.nl>                   +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/10/28 13:30:44 by jbennink      #+#    #+#                 */
/*   Updated: 2020/06/03 14:12:29 by jbennink      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

int	ft_atoi(const char *str)
{
	long	result;
	int		sign;
	long	max;

	max = 9223372036854775807;
	result = 0;
	sign = 1;
	while (*str == ' ' || *str == '\t' || *str == '\r'
			|| *str == '\n' || *str == '\v' || *str == '\f')
		str++;
	if (*str == '-' || *str == '+')
	{
		if (*str == '-')
			sign = -1;
		str++;
	}
	while (*str >= '0' && *str <= '9')
	{
		if (result > (max / 10))
			return (sign > 0) ? (-1) : (0);
		result = result * 10 + (*str - '0');
		str++;
	}
	return (result * sign);
}
