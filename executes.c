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

static bool	exec(char **args, char *exec_path)
{
	pid_t	pid;
	int		status;

	pid = fork();
	if (pid == 0)
	{
		if (execve(exec_path, args, g_shellvars.envvars) == -1)
		{
			g_shellvars.exitstatus = 127;
			return (false);
		}
	}
	else if (pid < 0)
		leaks_exit("error forking", 1);
	else
	{
		waitpid(pid, &status, WUNTRACED);
		while (!WIFEXITED(status) && !WIFSIGNALED(status))
			waitpid(pid, &status, WUNTRACED);
		if (WIFEXITED(status))
			g_shellvars.exitstatus = WEXITSTATUS(status);
		if (WIFSIGNALED(status))
			g_shellvars.exitstatus = WTERMSIG(status) + 128;
	}
	return (true);
}

static bool	find_path_execs(char **args)
{
	char		**path;
	char		*paths;
	char		*executable;
	char		*exec_path;
	int			i;
	struct stat	buff;

	paths = find_env("PATH");
	path = ft_split(paths, ':');
	free(paths);
	malloc_check(path);
	executable = ft_strjoin("/", args[0]);
	malloc_check(executable);
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
	free_array(path);
	free(executable);
	g_shellvars.exitstatus = 127;
	return (false);
}

bool		find_executables(char **args)
{
	if (ft_strchr(args[0], '/'))
	{
		if (!exec(args, args[0]))
		{
			write(1, "no such file or directory\n", 26);
			return (false);
		}
		return (true);
	}
	return (find_path_execs(args));
}