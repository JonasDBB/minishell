/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   redirections.c                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: jbennink <jbennink@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/10/20 13:38:48 by jbennink      #+#    #+#                 */
/*   Updated: 2020/10/20 13:38:48 by jbennink      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../incl/minishell.h"

static void	remove_args(char **args, int i)
{
	free(args[i]);
	free(args[i + 1]);
	while (args[i + 2])
	{
		args[i] = args[i + 2];
		i++;
	}
	args[i] = NULL;
}

static int	check_if_redir(char **tokens, int *fd_out, int *fd_in, int i)
{
	if (tokens[i][0] == redirect_trunc)
	{
		*fd_out = set_fd_out(tokens[i + 1], O_TRUNC, *fd_out);
		if (*fd_out == -1)
			return (0);
		remove_args(tokens, i);
		return (-1);
	}
	if (tokens[i][0] == redirect_append)
	{
		*fd_out = set_fd_out(tokens[i + 1], O_APPEND, *fd_out);
		if (*fd_out == -1)
			return (0);
		remove_args(tokens, i);
		return (-1);
	}
	if (tokens[i][0] == redirect_input)
	{
		*fd_in = set_fd_in(tokens[i + 1], *fd_in);
		if (*fd_in == -1)
			return (0);
		remove_args(tokens, i);
		return (-1);
	}
	return (1);
}

bool		find_redirects(char **tokens)
{
	int	i;
	int fd_out;
	int	fd_in;
	int check;

	i = 0;
	fd_out = 0;
	fd_in = 0;
	while (tokens[i])
	{
		check = check_if_redir(tokens, &fd_out, &fd_in, i);
		if (!check)
			return (false);
		if (check == 1)
			i++;
	}
	if (fd_out)
		setfds(fd_out, STDOUT_FILENO);
	if (fd_in)
		setfds(fd_in, STDIN_FILENO);
	return (true);
}
