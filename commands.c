/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   commands.c                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: jbennink <jbennink@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/10/13 08:45:50 by jbennink      #+#    #+#                 */
/*   Updated: 2020/10/13 08:45:50 by jbennink      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*#include <stdio.h>
void		printcommandlist(t_list *commandlist)
{
	t_list *tmp;
	int i = 0;
	int c = 0;

	tmp = commandlist;
	while (tmp)
	{
		printf("\033[0;36m");
		if (c % 2)
			printf("\033[0;31m");
		i = 0;
		while (((t_command*)tmp->content)->tokens[i])
		{
			printf("\033[0;36m");
			if (i % 2)
				printf("\033[0;31m");
			printf("%s ", ((t_command*)tmp->content)->tokens[i]);
			i++;
		}
		if (((t_command*)tmp->content)->type == end)
			printf("\ntype is end\n");
		else
			printf("\ntype is %c\n", ((t_command*)tmp->content)->type);
		tmp = tmp->next;
		c++;
	}
	printf("\033[0m\n");
}
*/
bool		command_splitting(t_token *token)
{
	if (token->end != ' ')
		return (false);
	if (!ft_strchr(";|", token->string[0]))
		return (false);
	if (!token->string[0])
		return (false);
	return (true);
}

t_command	*new_command(t_list *tokenlist)
{
	t_command	*ret;
	t_list		*tmp;
	int			count;
	int			i;

	ret = malloc(sizeof(t_command));
	malloc_check(ret);
	tmp = tokenlist;
	count = 0;
	while (!command_splitting((t_token*)tmp->content) && tmp->next)
	{
		count++;
		tmp = tmp->next;
	}
	if (!tmp->next)
		count++;
	tmp = tokenlist;
	ret->tokens = malloc(sizeof(char*) * (count + 1));
	malloc_check(ret->tokens);
	i = 0;
	while (!command_splitting((t_token*)tmp->content) && tmp->next)
	{
		ret->tokens[i] = ft_strdup(((t_token*)tmp->content)->string);
		i++;
		tmp = tmp->next;
	}
	if (!tmp->next)
	{
		ret->tokens[i] = ft_strdup(((t_token*)tmp->content)->string);
		ret->type = end;
	}
	else
		ret->type = ((t_token*)tmp->content)->string[0];
	ret->tokens[count] = NULL;
	return (ret);
}

t_list		*commandtokens(t_list *tokenlist)
{
	t_list		*tmp;
	t_list		*new;
	t_list		*commandlist;

	tmp = tokenlist;
	commandlist = NULL;
	while (tmp)
	{
		new = ft_lstnew(new_command(tmp));
		ft_lstadd_back(&commandlist, new);
		while (!command_splitting((t_token*)tmp->content) && tmp->next)
			tmp = tmp->next;
		tmp = tmp->next;
	}
//	printcommandlist(commandlist);
	ft_lstclear(&tokenlist, free_one_token);
	return (commandlist);
}
