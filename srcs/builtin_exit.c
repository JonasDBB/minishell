/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   builtin_exit.c                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: jbennink <jbennink@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/10/25 15:21:51 by jbennink      #+#    #+#                 */
/*   Updated: 2020/10/25 15:21:51 by jbennink      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../incl/minishell.h"

static int	strisdigit(char *str)
{
	int	i;

	i = 0;
	while (ft_isspace(str[i]))
		i++;
	if (str[i] == '-' || str[i] == '+')
		i++;
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (0);
		i++;
	}
	return (1);
}

void		builtin_exit(char **args)
{
	if (args[1])
	{
		if (strisdigit(args[1]) && !args[2])
		{
			g_shell.exitstatus = (char)ft_atoi(args[1]);
			g_shell.loopstatus = 0;
		}
		else if (strisdigit(args[1]) && args[2])
		{
			write(2, g_shell.name, ft_strlen(g_shell.name));
			write(2, ": exit: too many arguments\n", 27);
		}
		else
		{
			write(2, g_shell.name, ft_strlen(g_shell.name));
			write(2, ": exit: ", 8);
			write(2, args[1], ft_strlen(args[1]));
			write(2, ": numeric argument required\n", 28);
			g_shell.exitstatus = 255;
			g_shell.loopstatus = 0;
		}
	}
	else
		g_shell.loopstatus = 0;
}
