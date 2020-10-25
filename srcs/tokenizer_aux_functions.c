/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   tokenizer_aux_functions.c                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: jbennink <jbennink@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/10/25 15:11:30 by jbennink      #+#    #+#                 */
/*   Updated: 2020/10/25 15:11:30 by jbennink      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../incl/minishell.h"

int	cmp_end_char(char c, char prev, char stop)
{
	if (stop == ' ')
	{
		if (c == '\'' || c == '\"' || c == '\t')
		{
			if (prev != escape)
				return (1);
		}
	}
	if (c == stop)
	{
		if (prev != escape)
			return (1);
	}
	return (0);
}

int	is_splitting_char(char c, char prev)
{
	if (ft_strchr(";|<>", c))
		if (prev != escape)
			return (1);
	return (0);
}

int	get_token_len(char *str, char stop, int it)
{
	int	i;

	i = it;
	if (i <= 0)
		i = 1;
	while (!cmp_end_char(str[i], str[i - 1], stop) && str[i])
		i++;
	if ((stop != ' ' && !str[i]) || str[ft_strlen(str) - 1] == escape)
	{
		g_shell.exitstatus = 1;
		write(2, "multiline commands are not supported\n", 37);
		return (-1);
	}
	return (i - it);
}
