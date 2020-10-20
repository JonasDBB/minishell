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

void		do_commands(t_list *commandlist)
{
	t_list		*tmp;
	t_command	*current;
	bool		known;

	tmp = commandlist;
	while (tmp)
	{
		current = (t_command*)tmp->content;
		if (!find_redirects(current->tokens))
			break ;
		if (!current->tokens[0])
		{
			tmp = tmp->next;
			g_shellvars.exitstatus = 1;
			dup2(g_shellvars.og_stdout, STDOUT_FILENO);
			dup2(g_shellvars.og_stdin, STDIN_FILENO);
			continue;
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
		dup2(g_shellvars.og_stdout, STDOUT_FILENO);
		dup2(g_shellvars.og_stdin, STDIN_FILENO);
	}
}
