/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   echo.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: jbennink <jbennink@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/06/24 11:44:18 by jbennink      #+#    #+#                 */
/*   Updated: 2020/06/24 13:24:54 by jbennink      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ms_echo(char *s)
{
	char	newline_flag;

	newline_flag = 1;
	while (*s == ' ')
		s++;
	if (*s == '-' && *(s + 1) == 'n')
	{
		newline_flag = 0;
		s += 2;
		while (*s == ' ')
			s++;
	}
	write(1, s, ft_strlen(s));
	if (newline_flag)
		write(1, "\n", 1);
}
