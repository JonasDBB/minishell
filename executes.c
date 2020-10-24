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

#include "minishell.h"

void		set_exit_from_child(int status)
{
	if (WIFEXITED(status))
		g_shell.exitstatus = WEXITSTATUS(status);
	if (WIFSIGNALED(status))
		g_shell.exitstatus = WTERMSIG(status) + 128;
}

static bool	exec(char **args, char *exec_path)
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
			g_shell.exitstatus = 127;
			return (false);
		}
	}
	else
	{
		waitpid(pid, &status, WUNTRACED);
		set_exit_from_child(status);
	}
	return (true);
}

static bool	find_exec(char **args, char **path, char *executable)
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

static bool	find_path_execs(char **args)
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

bool		find_executables(char **args)
{
	struct stat	buff;

	if (ft_strchr(args[0], '/'))
	{
		if (stat(args[0], &buff))
		{
			write(1, g_shell.name, ft_strlen(g_shell.name));
			write(1, ": ", 2);
			write(1, args[0], ft_strlen(args[0]));
			write(1, ": No such file or directory\n", 28);
			g_shell.exitstatus = 127;
		}
		else if (!exec(args, args[0]))
			write(1, "error executing\n", 16);
		return (true);
	}
	return (find_path_execs(args));
}
