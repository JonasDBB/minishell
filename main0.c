/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   main0.c                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: jbennink <jbennink@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/06/22 15:51:23 by jbennink      #+#    #+#                 */
/*   Updated: 2020/07/20 11:56:03 by jbennink      ########   odam.nl         */
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
	free(command);
}
#include <stdio.h>
int		main(void)
{
	char	*line;
	// int	fd = open("text.txt", O_RDONLY); //debug line, change gnl to read from fd and put command in text.txt
	while (1)
	{
		write(1, "minishell$ ", 11);
		get_next_line(1, &line);
		// get_next_line(fd, &line);
		do_everything(line);
		free(line);
	}
	return (0);
}
