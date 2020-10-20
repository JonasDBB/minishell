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

#include "minishell.h"

static int	set_fd_in(char *file, int old_fd)
{
	int		fd;

	if (old_fd)
		close(old_fd);
	fd = open(file, O_RDONLY, 0644);
	if (fd == -1)
	{
		write(1, g_shellvars.name, ft_strlen(g_shellvars.name));
		write(1, ": ", 2);
		write(1, file, ft_strlen(file));
		write(1, ": no such file or directory\n", 28);
	}
	return (fd);
}

static int	set_fd_out(char *file, int flag, int old_fd)
{
	int		fd;

	if (old_fd)
		close(old_fd);
	fd = open(file, O_CREAT | flag | O_WRONLY, 0644);
	if (fd == -1)
	{
		write(1, "error opening or creating file\n", 31);
	}
	return (fd);
}

static void	remove_args(char **args, int i)
{
	free(args[i]);
	free(args[i + 1]);
	while (args[i + 2])
	{
		args[i] = args[i + 2];
		i++;
	}
	args [i] = NULL;
}

bool		find_redirects(char **tokens)
{
	int	i;
	int fd_out;
	int	fd_in;

	i = 0;
	fd_out = 0;
	fd_in = 0;
	while (tokens[i])
	{
		if (tokens[i][0] == redirect_trunc)
		{
			fd_out = set_fd_out(tokens[i + 1], O_TRUNC, fd_out);
			if (fd_out == -1)
				return (false);
			remove_args(tokens, i);
			continue;
		}
		if (tokens[i][0] == redirect_append)
		{
			fd_out = set_fd_out(tokens[i + 1], O_APPEND, fd_out);
			if (fd_out == -1)
				return (false);
			remove_args(tokens, i);
			continue;
		}
		if (tokens[i][0] == redirect_input)
		{
			fd_in = set_fd_in(tokens[i + 1], fd_in);
			if (fd_in == -1)
				return (false);
			remove_args(tokens, i);
			continue;
		}
		i++;
	}
	if (fd_out)
	{
		dup2(fd_out, STDOUT_FILENO);
		close(fd_out);
	}
	if (fd_in)
	{
		dup2(fd_in, STDIN_FILENO);
		close(fd_in);
	}
	return (true);
}
