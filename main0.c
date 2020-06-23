/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   main0.c                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: jbennink <jbennink@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/06/22 15:51:23 by jbennink      #+#    #+#                 */
/*   Updated: 2020/06/23 11:33:36 by jbennink      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	do_everything(char *line)
{
	if (!strcmp(line, "exit"))
		exit(0);
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
