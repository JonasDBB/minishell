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

int 	builtin_pwd(void)
{
	char	*path;

	path = getcwd(NULL, 0);
	malloc_check(path);
	write(1, path, ft_strlen(path));
	write(1, "\n", 1);
	free(path);
	g_shellvars.exitstatus = 0;
	return (0);
}

int		builtin_echo(char **args)
{
	int		i;
	bool	newl;

	i = 1;
	newl = true;
	if (!ft_strcmp(args[1], "-n"))
	{
		newl = false;
		i++;
	}
	while (args[i])
	{
		write(1, args[i], ft_strlen(args[i]));
		if (args[i + 1])
			write (1, " ", 1);
		i++;
	}
	if (newl)
		write(1, "\n", 1);
	g_shellvars.exitstatus = 0;
	return (0);
}

int		builtin_cd(char *arg)
{
	int	ret;

	ret = chdir(arg);
	if (ret == -1)
	{
		write(1, "cd: no such file or directory: ", 31);
		write(1, arg, ft_strlen(arg));
		write(1, "\n", 1);
		g_shellvars.exitstatus = 1;
		return (1);
	}
	g_shellvars.exitstatus = 0;
	return (0);
}

int		builtin_env(char *arg)
{
	int	i;

	i = 0;
	if (arg)
	{
		write(1, "env: ", 5);
		write(1, arg, ft_strlen(arg));
		write(1, ": No such file or directory\n", 28);
		g_shellvars.exitstatus = 127;
		return (127);
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
	return (0);
}

int		strisdigit(char *str)
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

int		builtin_exit(char **args)
{
	if (args[1]) {
		if (strisdigit(args[1]) && !args[2])
		{
			g_shellvars.exitstatus = (char) ft_atoi(args[1]);
			g_shellvars.loopstatus = 0;
		}
		else if (strisdigit(args[1]) && args[2])
		{
			write(1, g_shellvars.name, ft_strlen(g_shellvars.name));
			write(1, ": exit: too many arguments\n", 27);
		}
		else
		{
			write(1, g_shellvars.name, ft_strlen(g_shellvars.name));
			write(1, ": exit: ", 8);
			write(1, args[1], ft_strlen(args[1]));
			write(1, ": numeric argument required\n", 28);
			g_shellvars.exitstatus = 255;
			g_shellvars.loopstatus = 0;
		}
	}
	else
		g_shellvars.loopstatus = 0;
	return (0);
}

bool	env_check(char *arg)
{
	int		i;
	bool	good;

	i = 1;
	good = true;
	if (!ft_isalpha(arg[0]) && arg[0] != '_')
		good = false;
	while(arg[i] != '=' && arg[i] && good)
	{
		if(!ft_isalnum(arg[i]) && arg[i] != '_')
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
	while(arg[i] && good)
	{
		if(!ft_isalnum(arg[i]) && arg[i] != '_')
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

int 	print_env_args(void)
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
	return (0);
}

int		builtin_export(char **args)
{
	int		count;
	int		i;
	char	**new;

	i = 1;
	g_shellvars.exitstatus = 0;
	if (!args[1])
		return (print_env_args());
	while (args[i])
	{
		if (!env_check(args[i]))
		{
			i++;
			continue;
		}
		count = 0;
		while (g_shellvars.envvars[count])
			count++;
		new = malloc(sizeof(char *) * (count + 2));
		malloc_check(new);
		new[count + 1] = NULL;
		count = 0;
		while (g_shellvars.envvars[count])
		{
			new[count] = ft_strdup(g_shellvars.envvars[count]);
			malloc_check(new[count]);
			count++;
		}
		new[count] = ft_strdup(args[i]);
		free_array(g_shellvars.envvars);
		g_shellvars.envvars = new;
		i++;
	}
	return (0);
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
int		builtin_unset(char **args)
{
	int		i;
	int		j;
	int		index_to_remove;
	int		count;
	char	**new;

	i = 1;
	g_shellvars.exitstatus = 0;
	while(args[i])
	{
		if (!env_check_unset(args[i]))
		{
			i++;
			continue;
		}
		index_to_remove = find_env_var(args[i]);
		if (index_to_remove == -1)
		{
			i++;
			continue;
		}
		count = 0;
		while (g_shellvars.envvars[count])
			count++;
		new = malloc(sizeof(char *) * (count));
		malloc_check(new);
		new[count - 1] = NULL;
		j = 0;
		count = 0;
		while (g_shellvars.envvars[count])
		{
			if (count != index_to_remove)
			{
				new[j] = ft_strdup(g_shellvars.envvars[count]);
				malloc_check(new[j]);
				j++;
			}
			count++;
		}
		free_array(g_shellvars.envvars);
		g_shellvars.envvars = new;
		i++;
	}
	return (0);
}

int		low_case_strcmp(char *s1, char *s2)
{
	int	i;

	i = 0;
	while (s1[i])
	{
		s1[i] = (char)ft_tolower(s1[i]);
		i++;
	}
	return (ft_strcmp(s1, s2));
}

void	do_commands(t_list *commandlist)
{
	t_list		*tmp;
	t_command	*current;

	tmp = commandlist;
	while (tmp)
	{
		current = (t_command*)tmp->content;
		if (!low_case_strcmp(current->tokens[0], "echo"))
			builtin_echo(current->tokens);
		else if (!ft_strcmp(current->tokens[0], "cd"))
			builtin_cd(current->tokens[1]);
		else if (!low_case_strcmp(current->tokens[0], "pwd"))
			builtin_pwd();
		else if (!ft_strcmp(current->tokens[0], "export"))
			builtin_export(current->tokens);
		else if (!ft_strcmp(current->tokens[0], "unset"))
			builtin_unset(current->tokens);
		else if (!low_case_strcmp(current->tokens[0], "env"))
			builtin_env(current->tokens[1]);
		else if (!ft_strcmp(current->tokens[0], "exit"))
			builtin_exit(current->tokens);
		else
		{
			write(1, g_shellvars.name, ft_strlen(g_shellvars.name));
			write(1, ": ", 2);
			write(1, current->tokens[0], ft_strlen(current->tokens[0]));
			write(1, ": command not found\n", 20);
		}
		tmp = tmp->next;
	}
}