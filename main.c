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
//	system("leaks Minishell > leaks.txt");
//	system("grep \"total leaked bytes\" leaks.txt");
//	system("rm leaks.txt");
	exit(exitcode);
}

void 		concat_list(t_list *tokenlist)
{
	t_list		*current;
//	t_list		*tmp;
	t_tokens	this;
//	t_tokens 	to_add;
//	char 		*newstr;

	current = tokenlist;
	while (current)
	{
		printf("size is %i\n", ft_lstsize(tokenlist));
		this = *(t_tokens*)current->content;
		if (!this.space_after && current->next)
		{


//			FAK THIS SHITE

//			to_add = *(t_tokens*)current->next->content;
//			newstr = ft_strjoin(this.content, to_add.content);
//			malloc_check(newstr);
//			free(this.content);
//			this.content = newstr;
//			tmp = current->next;
//			this.space_after = to_add.space_after;
//			current->next = current->next->next;
//			free(to_add.content);
//			free(tmp->content);
//			free(tmp);
//			free(newstr);

//			END OF SHITE
		}
		else
			current = current->next;
	}
	printf("size after is %i\n", ft_lstsize(tokenlist));
}

void 		printlist(t_list *tknlist)
{
	t_list	*tmp;
	int i = 0;

	tmp = tknlist;
	printf("         -|"); // to allign output
	while (tmp)
	{
		unsetescape(((t_tokens*)tmp->content)->content);
		printf("\033[0;36m");
		if (i % 2)
			printf("\033[0;31m");
		printf("%s", ((t_tokens*)tmp->content)->content);
		if (((t_tokens*)tmp->content)->space_after == true)
			printf(" ");
		tmp = tmp->next;
		i++;
	}
	printf("\033[0m|-\n");
}

void		ft_free_one(void *token)
{
	free(((t_tokens*)token)->content);
	free(token);
}

void		do_everything(char *line)
{
	t_list	*tknlist;

	tknlist = tokenizer(line);
	if (!tknlist)
		return ;
	// do expansions
//	concat_list(tknlist);
	printlist(tknlist);
	if (!ft_strcmp("exit", ((t_tokens*)tknlist->content)->content))
		leaks_exit("", 0);
	ft_lstclear(&tknlist, ft_free_one);
}

int			main(int ac, char **av, char **envp)
{
	char	*line;
	if (ac > 1)
		leaks_exit("Running with arguments is not supported.", 1);
	(void)av;
	(void)envp;
	while (1)
	{
		write(1, "minishell$ ", 11);
		get_next_line(0, &line);
		do_everything(line);
		free(line);
	}
//	return (0);
}
