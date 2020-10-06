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

void 		malloc_check(void *p)
{
	if (!p)
		leaks_exit("malloc2 fail", 1);
}

void		leaks_exit(char *error, int exitcode)
{
	if (exitcode)
	{
		write(2, "error: ",7);
		write(2, error, ft_strlen(error));
		write(2, "\n\n", 2);
	}
	system("leaks Minishell > leaks.txt");
	system("grep \"total leaked bytes\" leaks.txt");
	system("rm leaks.txt");
	exit(exitcode);
}

void 		concat_list(t_list *tokenlist)
{
	t_list		*current;
	t_list		*next_elem;
	t_tokens	*current_token;
	t_tokens 	*next_token;
	char 		*newstr;

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

			// copying over data from next to current
			current_token->space_after = next_token->space_after;
			newstr = ft_strjoin(current_token->string, next_token->string);
			malloc_check(newstr);
			free(current_token->string);
			current_token->string = ft_strdup(newstr);
			malloc_check(current_token->string);
			free(newstr);

			// setting current->next to skip over next_elem and deleting next_elem
			current->next = current->next->next;
			free_one_token(next_elem->content);
			free(next_elem);
		}
		else
			current = current->next;
	}
}

void 		printlist(t_list *tknlist)
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

void		free_one_token(void *token)
{
	free(((t_tokens*)token)->string);
	free(token);
}

void		do_everything(char *line)
{
	t_list	*tknlist;

	if (!ft_strlen(line))
		return ;
	tknlist = tokenizer(line);
	if (!tknlist
	)
		return ;
//	do expansions
	concat_list(tknlist);
	printlist(tknlist);
	if (!ft_strcmp("exit", ((t_tokens*)tknlist->content)->string))
		leaks_exit("", 0);
	ft_lstclear(&tknlist, free_one_token);
}

int			main(int ac, char **av, char **envp)
{
	char	*line;
	int 	gnl;

	if (ac > 1)
		leaks_exit("Running with arguments is not supported.", 1);
	(void)av;
	(void)envp;
	gnl = 1;
	while (gnl)
	{
		write(1, "minishell$ ", 11);
		gnl = get_next_line(0, &line);
		do_everything(line);
		free(line);
	}
	write (1, "exit\n", 5);
	return (0);
}
