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

#include "../incl/minishell.h"

void		builtin_pwd(void)
{
	char	*path;

	path = getcwd(NULL, 0);
	malloc_check(path);
	write(1, path, ft_strlen(path));
	write(1, "\n", 1);
	free(path);
	g_shell.exitstatus = 0;
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
	g_shell.exitstatus = 0;
}

static int	cd_no_args(void)
{
	char	*home;
	int		ret;

	home = find_env("HOME");
	if (!ft_strcmp(home, ""))
	{
		write(2, g_shell.name, ft_strlen(g_shell.name));
		write(2, ": ", 2);
		write(2, "cd: HOME not set\n", 17);
		g_shell.exitstatus = 1;
		free(home);
		return (-2);
	}
	ret = chdir(home);
	free(home);
	return (ret);
}

void		builtin_cd(char **args)
{
	int		ret;

	if (!args[1])
		ret = cd_no_args();
	else
		ret = chdir(args[1]);
	if (ret == -2)
		return ;
	if (ret == -1)
	{
		write(2, "cd: ", 4);
		write(2, args[1], ft_strlen(args[1]));
		write(2, " no such file or directory", 26);
		write(2, "\n", 1);
		g_shell.exitstatus = 1;
		return ;
	}
	g_shell.exitstatus = 0;
}
