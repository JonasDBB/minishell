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

static void	do_parent(t_command *curr, t_command *prev, pid_t *pids, pid_t pid)
{
	int	i;
	int status;

	close(curr->pipe_fds[1]);
	if (curr->type != '|')
		waitpid(pid, &status, WUNTRACED);
	else
	{
		i = 0;
		while (pids[i])
			i++;
		pids[i] = pid;
	}
	if (curr->type != '|')
		close(curr->pipe_fds[0]);
	if (prev && prev->type == '|')
		close(prev->pipe_fds[0]);
	else
		set_exit_from_child(status);
}

static void	child(t_command *current, t_command *previous)
{
	g_shell.is_child = true;
	if (current->type == '|')
	{
		if (dup2(current->pipe_fds[1], STDOUT_FILENO) == -1)
			leaks_exit("error dupping in pipe", -1);
		close(current->pipe_fds[1]);
	}
	if (previous && previous->type == '|')
	{
		if (dup2(previous->pipe_fds[0], STDIN_FILENO) == -1)
			leaks_exit("error dupping in pipe", -1);
		close(previous->pipe_fds[0]);
	}
	find_command(current);
	exit(g_shell.exitstatus);
}

void		create_pipe(t_command *current, t_command *previous, pid_t *pids)
{
	pid_t	pid;

	if (pipe(current->pipe_fds) == -1)
		leaks_exit("error creating pipe", -1);
	pid = fork();
	if (pid < 0)
		leaks_exit("error forking", 1);
	else if (pid == 0)
		child(current, previous);
	else
		do_parent(current, previous, pids, pid);
}
