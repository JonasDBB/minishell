/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   prepwork.c                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: jbennink <jbennink@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/10/25 15:17:20 by jbennink      #+#    #+#                 */
/*   Updated: 2020/10/25 15:17:20 by jbennink      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../incl/minishell.h"

char	**malloc_vars(char **envp)
{
	char	**result;
	int		count;

	count = 0;
	while (envp[count])
		count++;
	result = malloc(sizeof(char*) * (count + 1));
	malloc_check(result);
	count = 0;
	while (envp[count])
	{
		result[count] = ft_strdup(envp[count]);
		malloc_check(result[count]);
		count++;
	}
	result[count] = NULL;
	return (result);
}

void	prep_global(char *const *av, char **envp)
{
	g_shell.envvars = malloc_vars(envp);
	g_shell.exitstatus = 0;
	g_shell.loopstatus = 1;
	g_shell.name = ft_substr(ft_strrchr(av[0], '/'), 1, ft_strlen(av[0]) - 1);
	malloc_check(g_shell.name);
	g_shell.og_stdout = dup(STDOUT_FILENO);
	g_shell.og_stdin = dup(STDIN_FILENO);
	if (g_shell.og_stdout == -1 || g_shell.og_stdin == -1)
		leaks_exit("error dupping original filestreams", -1);
	g_shell.is_child = false;
	signal(SIGQUIT, handle_sig);
	signal(SIGINT, handle_sig);
}
