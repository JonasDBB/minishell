/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   main0.c                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: jbennink <jbennink@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/06/22 15:51:23 by jbennink      #+#    #+#                 */
/*   Updated: 2020/06/24 13:18:25 by jbennink      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	do_everything(char *line)
{
	char	*command;
	int		i;

	i = 0;
	while (line[i] != ' ' && line[i])
		i++;
	command = malloc(i + 1);
	i = 0;
	while (line[i] != ' ' && line[i])
	{
		command[i] = line[i];
		i++;
	}
	if (!ft_strcmp(command, "exit"))
		exit(0);
	else if (!ft_strcmp(command, "echo"))
		ms_echo(line + 4);
	else
	{
		write(1, "minishell: command not found: ", 30);
		write(1, line, ft_strlen(line));
		write(1, "\n", 1);
	}
}

int		main(void)
{
	char	*line;

	while (1)
	{
		write(1, "minishell$ ", 11);
		get_next_line(1, &line);
		do_everything(line);
		free(line);
	}
	return (0);
}
