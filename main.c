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
	t_tokens	*current_token;
	t_tokens	*next_token;

	current = tokenlist;
	while (current)
	{
		// creating current_token from current elem
		current_token = (t_tokens*)current->content;
		if (!current_token->space_after && current->next)
		{
			// creating next_token from next_elem
			next_elem = current->next;
			next_token = (t_tokens*)next_elem->content;

			// check if either token is a splitting character
			if (ft_strchr(";|<>", current_token->string[0]) || ft_strchr(";|<>", next_token->string[0]))
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

void	print_token_list(t_list *tknlist)
{
	t_list	*tmp;
	int i = 0;

	tmp = tknlist;
	printf("         -|"); // to allign output
	while (tmp)
	{
		unsetescape(((t_tokens*)tmp->content)->string);
		printf("\033[0;36m");
		if (i % 2)
			printf("\033[0;31m");
		printf("%s", ((t_tokens*)tmp->content)->string);
		if (((t_tokens*)tmp->content)->space_after == true)
			printf(" ");
		tmp = tmp->next;
		i++;
	}
	printf("\033[0m|-\n");
}


void	do_everything(char *line)
{
	t_list	*tokenlist;

	if (!ft_strlen(line))
		return ;
	tokenlist = tokenizer(line);
	if (!tokenlist)
		return ;
	expand_env_var(tokenlist);
	concat_list(tokenlist);
	create_append(tokenlist);

	if (syntax_check(tokenlist))
	{
		print_token_list(tokenlist);
		if (!ft_strcmp("exit", ((t_tokens *) tokenlist->content)->string))
			leaks_exit("", 0);
	}
	ft_lstclear(&tokenlist, free_one_token);
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
	while (g_shellvars.loopstatus)
	{
		write(1, g_shellvars.name, ft_strlen(g_shellvars.name));
		write(1, "$ ", 2);
		g_shellvars.loopstatus = get_next_line(0, &line);
		do_everything(line);
		free(line);
	}
	write (1, "exit\n", 5);
	leaks_exit("", g_shellvars.exitstatus);
}
