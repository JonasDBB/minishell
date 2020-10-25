/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   executes.c                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: jbennink <jbennink@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/10/20 13:41:41 by jbennink      #+#    #+#                 */
/*   Updated: 2020/10/20 13:41:41 by jbennink      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../incl/minishell.h"

static	void	exec_error_msg(char *arg)
{
	char	*errorstr;

	write(2, g_shell.name, ft_strlen(g_shell.name));
	write(2, ": ", 2);
	write(2, arg, ft_strlen(arg));
	write(2, ": ", 2);
	errorstr = strerror(errno);
	write(2, errorstr, ft_strlen(errorstr));
}

void			exec(char **args, char *exec_path)
{
	pid_t	pid;
	int		status;

	pid = 0;
	if (!g_shell.is_child)
		pid = fork();
	if (pid < 0)
		leaks_exit("error forking", 1);
	else if (pid == 0)
	{
		if (execve(exec_path, args, g_shell.envvars) == -1)
		{
			exec_error_msg(args[0]);
			exit(126);
		}
	}
	else
	{
		waitpid(pid, &status, WUNTRACED);
		set_exit_from_child(status);
	}
}

static bool		find_exec(char **args, char **path, char *executable)
{
	int			i;
	char		*exec_path;
	struct stat	buff;

	i = 0;
	while (path[i])
	{
		exec_path = ft_strjoin(path[i], executable);
		malloc_check(exec_path);
		if (!stat(exec_path, &buff))
		{
			exec(args, exec_path);
			free(exec_path);
			free_array(path);
			free(executable);
			return (true);
		}
		free(exec_path);
		i++;
	}
	return (false);
}

static bool		find_path_execs(char **args)
{
	char		**path;
	char		*paths;
	char		*executable;

	paths = find_env("PATH");
	path = ft_split(paths, ':');
	free(paths);
	malloc_check(path);
	executable = ft_strjoin("/", args[0]);
	malloc_check(executable);
	if (find_exec(args, path, executable))
		return (true);
	free_array(path);
	free(executable);
	g_shell.exitstatus = 127;
	return (false);
}

bool			find_executables(char **args)
{
	struct stat	buff;

	if (ft_strchr(args[0], '/'))
	{
		if (stat(args[0], &buff))
		{
			write(2, g_shell.name, ft_strlen(g_shell.name));
			write(2, ": ", 2);
			write(2, args[0], ft_strlen(args[0]));
			write(2, ": No such file or directory\n", 28);
			g_shell.exitstatus = 127;
		}
		else
			exec(args, args[0]);
		return (true);
	}
	return (find_path_execs(args));
}
