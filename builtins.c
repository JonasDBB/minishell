/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   builtins.c                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: jbennink <jbennink@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/10/16 13:57:13 by jbennink      #+#    #+#                 */
/*   Updated: 2020/10/16 13:57:13 by jbennink      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void		builtin_pwd(void)
{
	char	*path;

	path = getcwd(NULL, 0);
	malloc_check(path);
	write(1, path, ft_strlen(path));
	write(1, "\n", 1);
	free(path);
	g_shellvars.exitstatus = 0;
}

static bool	is_newline_option(char *str)
{
	int	i;

	i = 1;
	if (str[0] != '-' || !str[1])
		return (false);
	while (str[i])
	{
		if (str[i] != 'n')
			return (false);
		i++;
	}
	return (true);
}

void		builtin_echo(char **args)
{
	int		i;
	bool	newl;

	i = 1;
	newl = true;
	while (args[i] && is_newline_option(args[i]))
	{
		newl = false;
		i++;
	}
	while (args[i])
	{
		write(1, args[i], ft_strlen(args[i]));
		if (args[i + 1])
			write(1, " ", 1);
		i++;
	}
	if (newl)
		write(1, "\n", 1);
	g_shellvars.exitstatus = 0;
}

void		builtin_cd(char **args)
{
	int		ret;
	char	*home;

	if (!args[1])
	{
		home = find_env("HOME");
		if (!ft_strcmp(home, ""))
		{
			write(1, g_shellvars.name, ft_strlen(g_shellvars.name));
			write(1, ": ", 2);
			write(1, "cd: HOME not set\n", 17);
			g_shellvars.exitstatus = 1;
			free(home);
			return ;
		}
		ret = chdir(home);
		free(home);
	}
	else
		ret = chdir(args[1]);
	if (ret == -1)
	{
		write(1, "cd: no such file or directory: ", 31);
		write(1, args[1], ft_strlen(args[1]));
		write(1, "\n", 1);
		g_shellvars.exitstatus = 1;
		return ;
	}
	g_shellvars.exitstatus = 0;
}

void		builtin_env(char *arg)
{
	int	i;

	i = 0;
	if (arg)
	{
		write(1, "env: ", 5);
		write(1, arg, ft_strlen(arg));
		write(1, ": No such file or directory\n", 28);
		g_shellvars.exitstatus = 127;
		return ;
	}
	while (g_shellvars.envvars[i])
	{
		if (ft_strchr(g_shellvars.envvars[i], '='))
		{
			write(1, g_shellvars.envvars[i], ft_strlen(g_shellvars.envvars[i]));
			write(1, "\n", 1);
		}
		i++;
	}
	g_shellvars.exitstatus = 0;
}
