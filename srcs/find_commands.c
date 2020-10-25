/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   find_commands.c                                    :+:    :+:            */
/*                                                     +:+                    */
/*   By: jbennink <jbennink@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/10/25 15:34:02 by jbennink      #+#    #+#                 */
/*   Updated: 2020/10/25 15:34:02 by jbennink      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../incl/minishell.h"

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
	write(2, g_shell.name, ft_strlen(g_shell.name));
	write(2, ": ", 2);
	write(2, cmd, ft_strlen(cmd));
	write(2, ": command not found\n", 20);
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
