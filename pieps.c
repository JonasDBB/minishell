/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   pieps.c                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: jbennink <jbennink@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/10/21 08:08:56 by jbennink      #+#    #+#                 */
/*   Updated: 2020/10/21 08:08:56 by jbennink      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <stdio.h>

void	create_pipe(t_command *current, t_command *previous, pid_t *pids)
{
	pid_t	pid;
	int 	status;

	pipe(current->pipe_fds);
	pid = fork();
	if (pid < 0)
		leaks_exit("error forking", 1);
	else if (pid == 0)
	{
		g_shellvars.is_child = true;
		if (current->type == '|')
		{
			dup2(current->pipe_fds[1], STDOUT_FILENO);
			close(current->pipe_fds[1]);
		}
		if (previous && previous->type == '|')
		{
			dup2(previous->pipe_fds[0], STDIN_FILENO);
			close(previous->pipe_fds[0]);
		}
		do_cmnds(current);
		exit(g_shellvars.exitstatus);
	}
	else
	{
		close(current->pipe_fds[1]);
		if (current->type != '|')
			waitpid(pid, &status, WUNTRACED);
		else
		{
			int i = 0;
			while (pids[i])
				i++;
			pids[i] = pid;
		}
		if (current->type != '|')
			close(current->pipe_fds[0]);
		if (previous && previous->type == '|')
			close(previous->pipe_fds[0]);
		if (WIFEXITED(status))
			g_shellvars.exitstatus = WEXITSTATUS(status);
		if (WIFSIGNALED(status))
			g_shellvars.exitstatus = WTERMSIG(status) + 128;
	}
}
