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
	int	i;

	i = 0;
	while (s1[i])
	{
		s1[i] = (char)ft_tolower(s1[i]);
		i++;
	}
	return (ft_strcmp(s1, s2));
}

void		do_commands(t_list *commandlist)
{
	t_list		*tmp;
	t_command	*current;

	tmp = commandlist;
	while (tmp)
	{
		current = (t_command*)tmp->content;
		if (!low_case_strcmp(current->tokens[0], "echo"))
			builtin_echo(current->tokens);
		else if (!ft_strcmp(current->tokens[0], "cd"))
			builtin_cd(current->tokens[1]);
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
			write(1, g_shellvars.name, ft_strlen(g_shellvars.name));
			write(1, ": ", 2);
			write(1, current->tokens[0], ft_strlen(current->tokens[0]));
			write(1, ": command not found\n", 20);
		}
		tmp = tmp->next;
	}
}
