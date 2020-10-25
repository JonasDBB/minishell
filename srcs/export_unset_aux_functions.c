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

#include "../incl/minishell.h"

bool		env_check(char *arg)
{
	int		i;
	bool	good;

	i = 1;
	good = true;
	if (!ft_isalpha(arg[0]) && arg[0] != '_')
		good = false;
	if (!arg[0])
		return (false);
	while (arg[i] != '=' && arg[i] && good)
	{
		if (!ft_isalnum(arg[i]) && arg[i] != '_')
			good = false;
		i++;
	}
	if (!good)
	{
		g_shell.exitstatus = 1;
		write(2, g_shell.name, ft_strlen(g_shell.name));
		write(2, ": export: `", 11);
		write(2, arg, ft_strlen(arg));
		write(2, "\': not a valid identifier\n", 26);
	}
	return (good);
}

bool		env_check_unset(char *arg)
{
	int		i;
	bool	good;

	i = 1;
	good = true;
	if (!ft_isalpha(arg[0]) && arg[0] != '_')
		good = false;
	if (!arg[0])
		return (false);
	while (arg[i] && good)
	{
		if (!ft_isalnum(arg[i]) && arg[i] != '_')
			good = false;
		i++;
	}
	if (!good)
	{
		g_shell.exitstatus = 1;
		write(2, g_shell.name, ft_strlen(g_shell.name));
		write(2, ": unset: `", 10);
		write(2, arg, ft_strlen(arg));
		write(2, "\': not a valid identifier\n", 26);
	}
	return (good);
}

void		print_one_env(char *var, int i)
{
	int		loc;
	char	*tmp;

	loc = find_loc(var, '=');
	if (loc == -1)
	{
		write(1, g_shell.envvars[i], ft_strlen(g_shell.envvars[i]));
		return ;
	}
	else
		write(1, g_shell.envvars[i], loc);
	write(1, "\"", 1);
	tmp = var;
	tmp += loc;
	write(1, tmp, ft_strlen(tmp));
	write(1, "\"", 1);
}

void		print_env_args(void)
{
	int	i;

	i = 0;
	while (g_shell.envvars[i])
	{
		write(1, "declare -x ", 11);
		print_one_env(g_shell.envvars[i], i);
		write(1, "\n", 1);
		i++;
	}
}
