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
	t_list		*current;
	t_list		*next_elem;
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

void	print_token_list(t_list *tokenlist)
{
	t_list	*tmp;
	int i = 0;

	tmp = tokenlist;
	printf("         -|"); // to allign output
	while (tmp)
	{
		unsetescape(((t_token*)tmp->content)->string);
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

void	do_everything(char *line)
{
	t_list	*tokenlist;
	t_list	*commands;

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
//	print_token_list(tokenlist);

	commands = commandtokens(tokenlist);
	do_commands(commands);
//	if (!ft_strcmp(((t_command*)commands->content)->tokens[0], "exit"))
//		leaks_exit("", 0);
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
	g_shellvars.exitmessage = ft_strdup("exit\n");
	while (g_shellvars.loopstatus)
	{
		signal(SIGQUIT, handle_sig);
		signal(SIGINT, handle_sig);
		prompt();
		g_shellvars.loopstatus = get_next_line(0, &line);
		do_everything(line);
		free(line);
	}
	write (1, g_shellvars.exitmessage, ft_strlen(g_shellvars.exitmessage));
	exit(g_shellvars.exitstatus);
}
