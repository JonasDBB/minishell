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

void		do_cmnds(t_command *current)
{
	if (!find_redirects(current->tokens) || !current->tokens[0])
	{
		if (current->tokens[0])
			g_shellvars.exitstatus = 1;
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
	else if (!low_case_strcmp(current->tokens[0], "env"))
		builtin_env(current->tokens[1]);
	else if (!ft_strcmp(current->tokens[0], "exit"))
		builtin_exit(current->tokens);
	else if (!find_executables(current->tokens))
		{
			write(1, g_shellvars.name, ft_strlen(g_shellvars.name));
			write(1, ": ", 2);
			write(1, current->tokens[0], ft_strlen(current->tokens[0]));
			write(1, ": command not found\n", 20);
		}
}

// this is to not do anything on catching sig in child process
void		ignoresig(int sig)
{
	(void)sig;
	write(1, "\n", 1);
}

void		do_commands(t_list *commandlist)
{
	t_list		*tmp;
	t_command	*current;
	t_command	*prev;
	t_list		*tmp_count;
	pid_t		*pids;
	int count;

	signal(SIGINT, ignoresig);
	signal(SIGQUIT, ignoresig);
	tmp = commandlist;
	pids = NULL;
	while (tmp)
	{
		current = (t_command*)tmp->content;
		prev = NULL;
		if (tmp->previous) {
			prev = (t_command*)tmp->previous->content;
		}
		//===============
		if (current->type == '|' && (!tmp->previous || prev->type != '|'))
		{
			pids = NULL;
			tmp_count = tmp;
			count = 0;
			while (((t_command *) tmp_count->content)->type == '|') {
				count++;
				tmp_count = tmp_count->next;
			}
			if (count)
				pids = malloc(sizeof(pid_t) * count);
			ft_bzero(pids, sizeof(pid_t) * count);
		}
		//===============


		if (current->type == '|' || (prev && prev->type == '|'))
			create_pipe(current, prev, pids);
		else
			do_cmnds(current);
		if (STDOUT_FILENO != g_shellvars.og_stdout)
			dup2(g_shellvars.og_stdout, STDOUT_FILENO);
		if (STDIN_FILENO != g_shellvars.og_stdin)
			dup2(g_shellvars.og_stdin, STDIN_FILENO);
		tmp = tmp->next;
		//===============

		if (current->type != '|')
		{
			int i = 0;
			while (pids && i < count)
			{
				wait(&pids[i]);
				i++;
			}
			free(pids);
			pids = NULL;
		}
		//===============
	}
	signal(SIGQUIT, handle_sig);
	signal(SIGINT, handle_sig);
}
