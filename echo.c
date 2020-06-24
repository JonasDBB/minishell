/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   echo.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: jbennink <jbennink@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/06/24 11:44:18 by jbennink      #+#    #+#                 */
/*   Updated: 2020/06/24 15:31:20 by jbennink      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ms_echo(char *s)
{
	char	newline_flag;
	int		i;

	newline_flag = 1;
	i = 0;
	while (*s == ' ')
		s++;
	if (*s == '-' && *(s + 1) == 'n' && *(s + 2) == ' ')
	{
		newline_flag = 0;
		s += 2;
		while (*s == ' ')
			s++;
	}
	while (*s)
	{
		write(1, s, 1);
		s++;
		while (*s == ' ')
			s++;
		if (*(s - 1) == ' ')
			write(1, " ", 1);
	}
	if (newline_flag)
		write(1, "\n", 1);
}
