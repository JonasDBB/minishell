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

#include "../incl/minishell.h"

/*
** this is to not do anything on catching sig in child process
*/

static void	ignoresig(int sig)
{
	(void)sig;
	write(1, "\n", 1);
}

static void	prep_pids(t_pipe_pids *pipe_pids, t_list *current_elem)
{
	t_list	*tmp;

	pipe_pids->pids = NULL;
	tmp = current_elem;
	pipe_pids->size = 0;
	while (((t_command*)tmp->content)->type == '|')
	{
		pipe_pids->size++;
		tmp = tmp->next;
	}
	if (pipe_pids->size)
	{
		pipe_pids->pids = malloc(sizeof(pid_t) * pipe_pids->size);
		malloc_check(pipe_pids->pids);
		ft_bzero(pipe_pids->pids, sizeof(pid_t) * pipe_pids->size);
	}
}

static void	wait_for_pipe_pids(t_pipe_pids *pipe_pids)
{
	int	i;

	i = 0;
	while (pipe_pids->pids && i < pipe_pids->size)
	{
		wait(&(pipe_pids->pids[i]));
		i++;
	}
	free(pipe_pids->pids);
	pipe_pids->pids = NULL;
}

void		do_one_command(t_list *tmp, t_pipe_pids *pipe_pids)
{
	t_command	*current;
	t_command	*prev;

	current = (t_command*)tmp->content;
	prev = NULL;
	if (tmp->previous)
		prev = (t_command*)tmp->previous->content;
	if (current->type == '|' && (!tmp->previous || prev->type != '|'))
		prep_pids(pipe_pids, tmp);
	if (current->type == '|' || (prev && prev->type == '|'))
		make_pipe(current, prev, (*pipe_pids).pids);
	else
		find_command(current);
	reset_fds();
	if (current->type != '|')
		wait_for_pipe_pids(pipe_pids);
}

void		do_command_list(t_list *commandlist)
{
	t_list		*tmp;
	t_pipe_pids	pipe_pids;

	signal(SIGINT, ignoresig);
	signal(SIGQUIT, ignoresig);
	tmp = commandlist;
	pipe_pids.pids = NULL;
	while (tmp && g_shell.loopstatus)
	{
		do_one_command(tmp, &pipe_pids);
		tmp = tmp->next;
	}
	signal(SIGQUIT, handle_sig);
	signal(SIGINT, handle_sig);
}
