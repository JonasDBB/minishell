/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   main0.c                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: jbennink <jbennink@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/06/22 15:51:23 by jbennink      #+#    #+#                 */
/*   Updated: 2020/07/23 15:55:51 by jbennink      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../incl/minishell.h"

/*
**void	print_token_list(t_list *tokenlist)
**{
**	t_list	*tmp;
**	int i = 0;
**	tmp = tokenlist;
**	printf("         -|"); // to allign output
**	while (tmp)
**	{
**		unsetescape(((t_token*)tmp->content)->str);
** //		unset_redirects(((t_token*)tmp->content)->str);
**		printf("\033[0;36m");
**		if (i % 2)
**			printf("\033[0;31m");
**		printf("=%s=", ((t_token*)tmp->content)->str);
**		if (((t_token*)tmp->content)->space_after == true)
**			printf(" ");
**		tmp = tmp->next;
**		i++;
**	}
**	printf("\033[0m|-\n");
**}
*/

static void	do_everything(char *line)
{
	t_list	*tokenlist;
	t_list	*commands;

	if (!ft_strlen(line))
		return ;
	tokenlist = tokenizer(line);
	if (tokenlist)
		ft_lstadd_front(&tokenlist, ft_lstnew(new_token("start", '\'', 'a')));
	if (!tokenlist)
		return ;
	create_append(tokenlist);
	if (!syntax_check(tokenlist))
	{
		ft_lstclear(&tokenlist, free_one_token);
		return ;
	}
	set_redirs(tokenlist);
	expand_env_var(tokenlist);
	retokenize_expanded_vars(tokenlist);
	concat_list(tokenlist);
	remove_escapes(tokenlist);
	commands = commandtokens(tokenlist);
	do_command_list(commands);
	ft_lstclear(&commands, free_one_command);
}

static void	prompt(void)
{
	write(2, g_shell.name, ft_strlen(g_shell.name));
	write(2, "$ ", 2);
}

void		handle_sig(int signal)
{
	char	deltwo[6];

	deltwo[0] = '\b';
	deltwo[1] = '\b';
	deltwo[2] = 127;
	deltwo[3] = 127;
	deltwo[4] = '\b';
	deltwo[5] = '\b';
	if (signal == SIGQUIT)
		write(1, deltwo, 6);
	if (signal == SIGINT)
	{
		g_shell.exitstatus = 1;
		write(1, deltwo, 6);
		write(1, "\n", 1);
		prompt();
	}
}

int			main(int ac, char **av, char **envp)
{
	char	*line;

	if (ac > 1)
		leaks_exit("Running with arguments is not supported.", 1);
	prep_global(av, envp);
	while (g_shell.loopstatus)
	{
		prompt();
		g_shell.loopstatus = get_next_line(0, &line);
		malloc_check(line);
		do_everything(line);
		free(line);
	}
	write(2, "exit\n", 5);
	exit(g_shell.exitstatus);
}
