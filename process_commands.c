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
#include <stdio.h>
void		do_cmnds(t_command *current)
{
//	printf("executing with first token %s\n", current->tokens[0]);
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
	if (g_shellvars.is_child)
	{
		exit(g_shellvars.exitstatus);
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
	pid_t		pid_child;
	int			status;
	int			piping;

	signal(SIGINT, ignoresig);
	signal(SIGQUIT, ignoresig);
	tmp = commandlist;
	while (tmp)
	{
		piping = 0;
		current = (t_command*)tmp->content;
		if (current->type == '|' || (tmp->previous && ((t_command*)tmp->previous->content)->type))
		{
			piping = 1;
			pid_child = pajp(current);
			waitpid(pid_child, &status, WUNTRACED);
			tmp = tmp->next;
		}
		do_cmnds(current);
		tmp = tmp->next;
		dup2(g_shellvars.og_stdout, STDOUT_FILENO);
		dup2(g_shellvars.og_stdin, STDIN_FILENO);
	}
	signal(SIGQUIT, handle_sig);
	signal(SIGINT, handle_sig);
}
