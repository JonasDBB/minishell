/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   err_and_exit.c                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: jbennink <jbennink@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/10/09 09:47:01 by jbennink      #+#    #+#                 */
/*   Updated: 2020/10/09 09:47:01 by jbennink      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	malloc_check(void *p)
{
	if (!p)
		leaks_exit("malloc2 fail", 1);
}

void	free_array(char **array)
{
	int	i;

	i = 0;
	while (array[i])
	{
		free(array[i]);
		i++;
	}
	free(array);
}
#include <stdio.h>
void	free_one_command(void *token)
{
	free_array(((t_command*)token)->tokens);
//	dprintf(g_shellvars.og_stdout, "errno before close %i\n", errno);
//	close(((t_command*)token)->pipe_fds[0]);
//	close(((t_command*)token)->pipe_fds[1]);
//	dprintf(g_shellvars.og_stdout, "errno after close %i\n", errno);
	free(token);
}

void	free_one_token(void *token)
{
	free(((t_token*)token)->string);
	free(token);
}

void	leaks_exit(char *error, int exitcode)
{

	if (error[0])
	{
		write(2, "error: ", 7);
		write(2, error, ft_strlen(error));
		write(2, "\n\n", 2);
	}
	char *tmp = ft_strjoin("leaks \"", g_shellvars.name);
	malloc_check(tmp);
	char *command = ft_strjoin(tmp, "\" >leaks.txt");
	malloc_check(command);
	free(tmp);
	system(command);
	free(command);
//	system("cat leaks.txt"); // for full display of leaks; comment out line below to use
	system("grep \"total leaked bytes\" leaks.txt"); // for short leaks; comment out line above
	system("rm leaks.txt");
	exit(exitcode);
}
