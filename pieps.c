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

pid_t	pajp(t_command *first)
{
	pid_t	pid_child;
	int		fd[2];

	if (pipe(fd) < 0)
		exit(69);
	pid_child = fork();
	if (pid_child < 0)
		leaks_exit("error forking", 1);
	if (pid_child == 0)
	{
		g_shellvars.is_child = true;
		close(fd[0]);
		dup2(fd[1], STDOUT_FILENO);
		close(fd[1]);
		do_cmnds(first);
		exit(0);
	}
	return (pid_child);
}
