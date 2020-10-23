/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   env_and_exit_builtins.c                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: jbennink <jbennink@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/10/17 09:03:05 by jbennink      #+#    #+#                 */
/*   Updated: 2020/10/17 09:03:05 by jbennink      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	strisdigit(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (0);
		i++;
	}
	return (1);
}

void		builtin_exit(char **args)
{
	if (args[1])
	{
		if (strisdigit(args[1]) && !args[2])
		{
			g_shell.exitstatus = (char)ft_atoi(args[1]);
			g_shell.loopstatus = 0;
		}
		else if (strisdigit(args[1]) && args[2])
		{
			write(1, g_shell.name, ft_strlen(g_shell.name));
			write(1, ": exit: too many arguments\n", 27);
		}
		else
		{
			write(1, g_shell.name, ft_strlen(g_shell.name));
			write(1, ": exit: ", 8);
			write(1, args[1], ft_strlen(args[1]));
			write(1, ": numeric argument required\n", 28);
			g_shell.exitstatus = 255;
			g_shell.loopstatus = 0;
		}
	}
	else
		g_shell.loopstatus = 0;
}

void		add_one_env_var(char *arg)
{
	int		count;
	char	**new;

	count = 0;
	while (g_shell.env[count])
		count++;
	new = malloc(sizeof(char *) * (count + 2));
	malloc_check(new);
	new[count + 1] = NULL;
	count = 0;
	while (g_shell.env[count])
	{
		new[count] = ft_strdup(g_shell.env[count]);
		malloc_check(new[count]);
		count++;
	}
	new[count] = ft_strdup(arg);
	malloc_check(new[count]);
	free_array(g_shell.env);
	g_shell.env = new;
}

void		builtin_export(char **args)
{
	int		i;

	i = 1;
	g_shell.exitstatus = 0;
	if (!args[1])
		return (print_env_args());
	while (args[i])
	{
		if (!env_check(args[i]))
		{
			i++;
			continue;
		}
		add_one_env_var(args[i]);
		i++;
	}
}

void		remove_one_env_var(int index_to_remove)
{
	int		count;
	int		i;
	char	**new;

	count = 0;
	while (g_shell.env[count])
		count++;
	new = malloc(sizeof(char *) * (count));
	malloc_check(new);
	new[count - 1] = NULL;
	i = 0;
	count = 0;
	while (g_shell.env[count])
	{
		if (count != index_to_remove)
		{
			new[i] = ft_strdup(g_shell.env[count]);
			malloc_check(new[i]);
			i++;
		}
		count++;
	}
	free_array(g_shell.env);
	g_shell.env = new;
}

void		builtin_unset(char **args)
{
	int		i;
	int		index_to_remove;

	i = 1;
	g_shell.exitstatus = 0;
	while (args[i])
	{
		index_to_remove = find_env_loc(args[i]);
		if (!env_check_unset(args[i]) || index_to_remove == -1)
		{
			i++;
			continue;
		}
		remove_one_env_var(index_to_remove);
		i++;
	}
}
