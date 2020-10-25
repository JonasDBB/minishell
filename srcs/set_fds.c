/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   set_redir_fds.c                                    :+:    :+:            */
/*                                                     +:+                    */
/*   By: jbennink <jbennink@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/10/25 15:29:30 by jbennink      #+#    #+#                 */
/*   Updated: 2020/10/25 15:29:30 by jbennink      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../incl/minishell.h"

int		set_fd_in(char *file, int old_fd)
{
	int		fd;

	if (old_fd)
		close(old_fd);
	fd = open(file, O_RDONLY, 0644);
	if (fd == -1)
	{
		write(2, g_shell.name, ft_strlen(g_shell.name));
		write(2, ": ", 2);
		write(2, file, ft_strlen(file));
		write(2, ": no such file or directory\n", 28);
	}
	return (fd);
}

int		set_fd_out(char *file, int flag, int old_fd)
{
	int		fd;

	if (old_fd)
		close(old_fd);
	fd = open(file, O_CREAT | flag | O_WRONLY, 0644);
	if (fd == -1)
		write(2, "error opening or creating file\n", 31);
	return (fd);
}

void	setfds(int fd_new, int std_fd)
{
	if (dup2(fd_new, std_fd) == -1)
		leaks_exit("error setting new fd in redirection", -1);
	close(fd_new);
}

void	reset_fds(void)
{
	if (STDOUT_FILENO != g_shell.og_stdout)
	{
		if (dup2(g_shell.og_stdout, STDOUT_FILENO) == -1)
			leaks_exit("error resetting stdout", -1);
	}
	if (STDIN_FILENO != g_shell.og_stdin)
	{
		if (dup2(g_shell.og_stdin, STDIN_FILENO) == -1)
			leaks_exit("error resetting stdout", -1);
	}
}
