/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   process_commands.c                                 :+:    :+:            */
/*                                                     +:+                    */
/*   By: jbennink <jbennink@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/10/17 09:21:00 by jbennink      #+#    #+#                 */
/*   Updated: 2020/10/17 09:21:00 by jbennink      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool		exec(char **args, char *exec_path)
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

bool		find_path_execs(char **args)
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
	}
	return (find_path_execs(args));
}

static int	low_case_strcmp(char *s1, char *s2)
{
	int		i;
	char	*s1lower;

	i = 0;
	s1lower = ft_strdup(s1);
	malloc_check(s1lower);
	while (s1lower[i])
	{
		s1lower[i] = (char)ft_tolower(s1lower[i]);
		i++;
	}
	i = ft_strcmp(s1lower, s2);
	free(s1lower);
	return (i);
}

void		do_commands(t_list *commandlist)
{
	t_list		*tmp;
	t_command	*current;
	bool		known;

	tmp = commandlist;
	while (tmp)
	{
		current = (t_command*)tmp->content;
		if (!low_case_strcmp(current->tokens[0], "echo"))
			builtin_echo(current->tokens);
		else if (!ft_strcmp(current->tokens[0], "cd"))
			builtin_cd(current->tokens);
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
			known = find_executables(current->tokens);
			if (!known)
			{
				write(1, g_shellvars.name, ft_strlen(g_shellvars.name));
				write(1, ": ", 2);
				write(1, current->tokens[0], ft_strlen(current->tokens[0]));
				write(1, ": command not found\n", 20);
			}
		}
		tmp = tmp->next;
	}
}
