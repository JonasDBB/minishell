/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   token_aux.c                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: jbennink <jbennink@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/10/09 09:57:10 by jbennink      #+#    #+#                 */
/*   Updated: 2020/10/09 09:57:10 by jbennink      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void			setescape(char *s)
{
	int i;

	i = 1;
	if (s[0] == '\\')
		s[0] = escape;
	while (s[i])
	{
		if (s[i] == '\\' && s[i - 1] != escape)
			s[i] = escape;
		i++;
	}
}

void			unsetescape(char *s)
{
	int i;

	i = 0;
	while (s[i])
	{
		if (s[i] == escape)
			s[i] = '\\';
		i++;
	}
}
