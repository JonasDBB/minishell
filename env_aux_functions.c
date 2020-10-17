/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   env_aux_functions.c                                :+:    :+:            */
/*                                                     +:+                    */
/*   By: jbennink <jbennink@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/10/17 08:25:53 by jbennink      #+#    #+#                 */
/*   Updated: 2020/10/17 08:25:53 by jbennink      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	env_check(char *arg)
{
	int		i;
	bool	good;

	i = 1;
	good = true;
	if (!ft_isalpha(arg[0]) && arg[0] != '_')
		good = false;
	while (arg[i] != '=' && arg[i] && good)
	{
		if (!ft_isalnum(arg[i]) && arg[i] != '_')
			good = false;
		i++;
	}
	if (!good)
	{
		g_shellvars.exitstatus = 1;
		write(1, g_shellvars.name, ft_strlen(g_shellvars.name));
		write(1, ": export: `", 11);
		write(1, arg, ft_strlen(arg));
		write(1, "\': not a valid identifier\n", 26);
	}
	return (good);
}

bool	env_check_unset(char *arg)
{
	int		i;
	bool	good;

	i = 1;
	good = true;
	if (!ft_isalpha(arg[0]) && arg[0] != '_')
		good = false;
	while (arg[i] && good)
	{
		if (!ft_isalnum(arg[i]) && arg[i] != '_')
			good = false;
		i++;
	}
	if (!good)
	{
		g_shellvars.exitstatus = 1;
		write(1, g_shellvars.name, ft_strlen(g_shellvars.name));
		write(1, ": unset: `", 10);
		write(1, arg, ft_strlen(arg));
		write(1, "\': not a valid identifier\n", 26);
	}
	return (good);
}

void	print_env_args(void)
{
	int	i;

	i = 0;
	while (g_shellvars.envvars[i])
	{
		write(1, "declare -x ", 11);
		write(1, g_shellvars.envvars[i], ft_strlen(g_shellvars.envvars[i]));
		write(1, "\n", 1);
		i++;
	}
}

int		strcmp_until_equals(char const *arg, char const *str)
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

int		find_env_var(char *arg)
{
	int	i;

	i = 0;
	while (g_shellvars.envvars[i])
	{
		if (strcmp_until_equals(arg, g_shellvars.envvars[i]))
			return (i);
		i++;
	}
	return (-1);
}
