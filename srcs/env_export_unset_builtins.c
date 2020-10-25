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

#include "../incl/minishell.h"

void		builtin_env(char *arg)
{
	int	i;

	i = 0;
	if (arg)
	{
		write(2, "envvars: ", 5);
		write(2, arg, ft_strlen(arg));
		write(2, ": No such file or directory\n", 28);
		g_shell.exitstatus = 127;
		return ;
	}
	while (g_shell.envvars[i])
	{
		if (ft_strchr(g_shell.envvars[i], '='))
		{
			write(1, g_shell.envvars[i], ft_strlen(g_shell.envvars[i]));
			write(1, "\n", 1);
		}
		i++;
	}
	g_shell.exitstatus = 0;
}

void		add_one_env_var(char *arg)
{
	int		count;
	char	**new;

	count = 0;
	while (g_shell.envvars[count])
		count++;
	new = malloc(sizeof(char *) * (count + 2));
	malloc_check(new);
	new[count + 1] = NULL;
	count = 0;
	while (g_shell.envvars[count])
	{
		new[count] = ft_strdup(g_shell.envvars[count]);
		malloc_check(new[count]);
		count++;
	}
	new[count] = ft_strdup(arg);
	malloc_check(new[count]);
	free_array(g_shell.envvars);
	g_shell.envvars = new;
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

static void	remove_one_env_var(int index_to_remove)
{
	int		count;
	int		i;
	char	**new;

	count = 0;
	while (g_shell.envvars[count])
		count++;
	new = malloc(sizeof(char *) * (count));
	malloc_check(new);
	new[count - 1] = NULL;
	i = 0;
	count = 0;
	while (g_shell.envvars[count])
	{
		if (count != index_to_remove)
		{
			new[i] = ft_strdup(g_shell.envvars[count]);
			malloc_check(new[i]);
			i++;
		}
		count++;
	}
	free_array(g_shell.envvars);
	g_shell.envvars = new;
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
