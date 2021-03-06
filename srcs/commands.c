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

#include "../incl/minishell.h"

static bool			is_command_splitting(t_token *token)
{
	if (token->end != ' ')
		return (false);
	if (!ft_strchr(";|", token->str[0]))
		return (false);
	if (!token->str[0])
		return (false);
	return (true);
}

static t_command	*prep_command_list(t_list *tokenlist)
{
	t_command	*ret;
	t_list		*tmp;
	int			count;

	ret = malloc(sizeof(t_command));
	malloc_check(ret);
	ret->pipe_fds[0] = -1;
	ret->pipe_fds[1] = -1;
	tmp = tokenlist;
	count = 0;
	while (!is_command_splitting((t_token*)tmp->content) && tmp->next)
	{
		count++;
		tmp = tmp->next;
	}
	if (!tmp->next && !is_command_splitting((t_token*)tmp->content))
		count++;
	ret->tokens = malloc(sizeof(char*) * (count + 1));
	malloc_check(ret->tokens);
	ret->tokens[count] = NULL;
	return (ret);
}

static t_command	*new_command(t_list *tokenlist)
{
	t_command	*ret;
	t_list		*tmp;
	int			i;

	ret = prep_command_list(tokenlist);
	tmp = tokenlist;
	i = 0;
	while (!is_command_splitting((t_token*)tmp->content) && tmp->next)
	{
		ret->tokens[i] = ft_strdup(((t_token*)tmp->content)->str);
		malloc_check(ret->tokens[i]);
		i++;
		tmp = tmp->next;
	}
	if (!tmp->next && !is_command_splitting((t_token*)tmp->content))
	{
		ret->tokens[i] = ft_strdup(((t_token*)tmp->content)->str);
		malloc_check(ret->tokens[i]);
		ret->type = last;
	}
	else
	{
		ret->type = ((t_token*)tmp->content)->str[0];
	}
	return (ret);
}

t_list				*expand_all(t_list **tmp)
{
	expand_env_var((*tmp));
	lst_add_before((*tmp), new_token("start", '\'', 'a'));
	(*tmp) = (*tmp)->previous;
	retokenize_expanded_vars((*tmp)->next);
	(*tmp) = (*tmp)->next;
	concat_list((*tmp));
	remove_escapes(tmp);
	return (*tmp);
}

void				commandtokens(t_list *tokenlist)
{
	t_list		*tmp;
	t_list		*new;
	t_list		*commandlist;

	tmp = tokenlist->next;
	commandlist = NULL;
	while (tmp)
	{
		tmp = expand_all(&tmp);
		if (!tmp)
			break ;
		new = ft_lstnew(new_command(tmp));
		malloc_check(new);
		ft_lstadd_back(&commandlist, new);
		if (((t_command*)new->content)->type != '|')
		{
			do_command_list(commandlist);
			ft_lstclear(&commandlist, free_one_command);
			commandlist = NULL;
		}
		while (!is_command_splitting((t_token*)tmp->content) && tmp->next)
			tmp = tmp->next;
		tmp = tmp->next;
	}
	ft_lstclear(&tokenlist, free_one_token);
}

/*
**#include <stdio.h>
**void		printcommandlist(t_list *commandlist)
**{
**	t_list *tmp;
**	int i = 0;
**	int c = 0;
**
**	tmp = commandlist;
**	while (tmp)
**	{
**		printf("\033[0;36m");
**		if (c % 2)
**			printf("\033[0;31m");
**		i = 0;
**		while (((t_command*)tmp->content)->tokens[i])
**		{
**			printf("\033[0;36m");
**			if (i % 2)
**				printf("\033[0;31m");
**			printf("%s ", ((t_command*)tmp->content)->tokens[i]);
**			i++;
**		}
**		if (((t_command*)tmp->content)->type == last)
**			printf("\ntype is last\n");
**		else
**			printf("\ntype is %c\n", ((t_command*)tmp->content)->type);
**		tmp = tmp->next;
**		c++;
**	}
**	printf("\033[0m\n");
**}
*/
