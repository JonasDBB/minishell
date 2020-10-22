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

#include "minishell.h"
#include <stdio.h>

void	concat_list(t_list *tokenlist)
{
	t_list	*current;
	t_list	*next_elem;
	t_token	*current_token;
	t_token	*next_token;

	current = tokenlist;
	while (current)
	{
		current_token = (t_token*)current->content;
		if (!current_token->space_after && current->next)
		{
			next_elem = current->next;
			next_token = (t_token*)next_elem->content;
			if (ft_strchr(";|<>", current_token->string[0])
				|| ft_strchr(";|<>", next_token->string[0]))
			{
				current = current->next;
				continue ;
			}
			merge_tokens(current, next_elem);
		}
		else
			current = current->next;
	}
}

static void		set_redirects(char *str)
{
	if (str[0] == '<')
		str[0] = redirect_input;
	if (str[0] == '>' && !str[1])
		str[0] = redirect_trunc;
	if (str[0] == '>' && str[1] == '>')
	{
		str[0] = redirect_append;
		str[1] = 0;
	}
}

/*
void	print_token_list(t_list *tokenlist)
{
	t_list	*tmp;
	int i = 0;

	tmp = tokenlist;
	printf("         -|"); // to allign output
	while (tmp)
	{
		unsetescape(((t_token*)tmp->content)->string);
//		unset_redirects(((t_token*)tmp->content)->string);
		printf("\033[0;36m");
		if (i % 2)
			printf("\033[0;31m");
		printf("%s", ((t_token*)tmp->content)->string);
		if (((t_token*)tmp->content)->space_after == true)
			printf(" ");
		tmp = tmp->next;
		i++;
	}
	printf("\033[0m|-\n");
}
*/
void	do_everything(char *line)
{
	t_list	*tokenlist;
	t_list	*commands;
	t_list	*tmp;
	t_list	*tmp1;
	t_token	*current;
	char	old[2];

	old[0] = escape;
	old[1] = 0;

	if (!ft_strlen(line))
		return ;
	tokenlist = tokenizer(line);
	if (!tokenlist)
		return ;
	expand_env_var(tokenlist);
	concat_list(tokenlist);
	create_append(tokenlist);
	if (!syntax_check(tokenlist))
	{
		ft_lstclear(&tokenlist, free_one_token);
		return ;
	}
	tmp1 = tokenlist;
	while(tmp1)
	{
		current = (t_token*)tmp1->content;
		if (current->end == ' ')
			set_redirects(current->string);
		tmp1 = tmp1->next;
	}
	tmp = tokenlist;
	while(tmp)
	{
		if (((t_token*)tmp->content)->end == '\"')
			unsetescapeif(((t_token*)tmp->content)->string);
		((t_token*)tmp->content)->string = ft_replace(((t_token*)tmp->content)->string, old, "");
		malloc_check(((t_token*)tmp->content)->string);
		tmp = tmp->next;
	}
	commands = commandtokens(tokenlist);
	do_commands(commands);
	ft_lstclear(&commands, free_one_command);
}

void	prompt(void)
{
	write(1, g_shellvars.name, ft_strlen(g_shellvars.name));
	write(1, "$ ", 2);
}

void	handle_sig(int signal)
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
		g_shellvars.exitstatus = 1;
		write(1, deltwo, 6);
		write(1, "\n", 1);
		prompt();
	}
}

int		main(int ac, char **av, char **envp)
{
	char	*line;
	if (ac > 1)
		leaks_exit("Running with arguments is not supported.", 1);
	g_shellvars.envvars = malloc_vars(envp);
	g_shellvars.exitstatus = 0;
	g_shellvars.loopstatus = 1;
	g_shellvars.name = ft_substr(ft_strrchr(av[0], '/'), 1, ft_strlen(av[0]) - 1);
	malloc_check(g_shellvars.name);
	g_shellvars.og_stdout = dup(STDOUT_FILENO);
	g_shellvars.og_stdin = dup(STDIN_FILENO);
	if (g_shellvars.og_stdout == -1 || g_shellvars.og_stdin == -1)
		leaks_exit("error dupping original filestreams", -1);
	g_shellvars.is_child = false;
	signal(SIGQUIT, handle_sig);
	signal(SIGINT, handle_sig);
	while (g_shellvars.loopstatus)
	{
		prompt();
		g_shellvars.loopstatus = get_next_line(0, &line);
		malloc_check(line);
		do_everything(line);
		free(line);
	}
	write (1, "exit\n", 5);
	exit(g_shellvars.exitstatus);
}
