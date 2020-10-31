/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   env_search_helper.c                                :+:    :+:            */
/*                                                     +:+                    */
/*   By: jbennink <jbennink@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/10/25 15:26:51 by jbennink      #+#    #+#                 */
/*   Updated: 2020/10/25 15:26:51 by jbennink      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../incl/minishell.h"

static int	strcmp_until_equals(char const *arg, char const *str)
{
	int	i;

	i = 0;
	while (arg[i] && str[i] != '=' && str[i])
	{
		if (arg[i] != str[i])
			return (0);
		i++;
	}
	if (!arg[i] && !str[i])
		return (1);
	if (!arg[i] && str[i] && str[i] != '=')
		return (0);
	if (!str[i] && arg[i])
		return (0);
	return (1);
}

int			find_env_loc(char *arg)
{
	int	i;

	i = 0;
	while (g_shell.envvars[i])
	{
		if (strcmp_until_equals(arg, g_shell.envvars[i]))
			return (i);
		i++;
	}
	return (-1);
}

void		remove_if_exists(char *envvar)
{
	int	loc;

	loc = find_env_loc(envvar);
	if (loc != -1)
		remove_one_env_var(loc);
}