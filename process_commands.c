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

static void	command_not_found(char *cmd)
{
	write(1, g_shell.name, ft_strlen(g_shell.name));
	write(1, ": ", 2);
	write(1, cmd, ft_strlen(cmd));
	write(1, ": command not found\n", 20);
}

void		find_command(t_command *current)
{
	if (!find_redirects(current->tokens) || !current->tokens[0])
	{
		if (current->tokens[0])
			g_shell.exitstatus = 1;
		return ;
	}
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
	else if (!low_case_strcmp(current->tokens[0], "envvars"))
		builtin_env(current->tokens[1]);
	else if (!ft_strcmp(current->tokens[0], "exit"))
		builtin_exit(current->tokens);
	else if (!find_executables(current->tokens))
		command_not_found(current->tokens[0]);
}

/*
** this is to not do anything on catching sig in child process
*/

void		ignoresig(int sig)
{
	(void)sig;
	write(1, "\n", 1);
}

void		prep_pids(t_pipe_pids *pipe_pids, t_list *current_elem)
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

void		reset_fds(void)
{
	if (STDOUT_FILENO != g_shell.og_stdout)
	{
		if (dup2(g_shell.og_stdout, STDOUT_FILENO) == -1)
			leaks_exit("error resetting stdout", -1);
	}
	if (STDIN_FILENO != g_shell.og_stdin)
	{
		if (dup2(g_shell.og_stdin, STDIN_FILENO) == -1)
			leaks_exit("error resetting stdout", -1);
	}
}

void		wait_for_pipe_pids(t_pipe_pids *pipe_pids)
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
		create_pipe(current, prev, (*pipe_pids).pids);
	else
		find_command(current);
	reset_fds();
	if (current->type != '|')
		wait_for_pipe_pids(pipe_pids);
}

void		do_commands(t_list *commandlist)
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
