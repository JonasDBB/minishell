/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   expansion_aux_functions.c                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: jbennink <jbennink@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/10/25 15:19:59 by jbennink      #+#    #+#                 */
/*   Updated: 2020/10/25 15:19:59 by jbennink      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../incl/minishell.h"

int		find_loc(char const *string, char c)
{
	int	i;

	i = 0;
	while (string[i] != c && string[i])
		i++;
	if (!string[i])
		return (-1);
	return (i + 1);
}

void	split_string(int i, int loc, char **before, char **after)
{
	(*before) = ft_substr(g_shell.envvars[i], 0, loc - 1);
	malloc_check((*before));
	(*after) = ft_substr(g_shell.envvars[i], loc,
					ft_strlen(g_shell.envvars[i]) - loc);
	malloc_check((*after));
}

char	*get_old_str(char *str, int loc)
{
	char	*res;
	int		i;

	i = 0;
	if (str[loc] == '_' || ft_isalpha(str[loc]))
	{
		while ((ft_isalnum(str[loc + i]) || str[loc + i] == '_')
				&& str[loc + i])
		{
			i++;
		}
	}
	if (!i)
		res = ft_strdup("");
	else
		res = ft_substr(str, loc, i);
	malloc_check(res);
	return (res);
}

void	unset_signs(char *string)
{
	int	i;

	i = 1;
	if (!string[0])
		return ;
	while (string[i])
	{
		if (string[i] == '$' && string[i - 1] == escape)
			string[i] = temp_escaped;
		i++;
	}
}

void	reset_signs(char *string)
{
	int	i;

	i = 0;
	while (string[i])
	{
		if (string[i] == temp_escaped)
			string[i] = '$';
		i++;
	}
}
