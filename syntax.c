/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   syntax.c                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: jbennink <jbennink@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/10/09 11:00:28 by jbennink      #+#    #+#                 */
/*   Updated: 2020/10/09 11:00:28 by jbennink      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <stdio.h>
bool		is_splitting(t_token *token)
{
	if (token->end != ' ')
		return (false);
	if (!ft_strchr(";|<>", token->string[0]))
		return (false);
	if (!token->string[0])
		return (false);
	return (true);
}

static bool	syntax_error(char *token)
{
	write(2, g_shellvars.name, ft_strlen(g_shellvars.name));
	write(2, ": syntax error near unexpected token `", 38);
	write(2, token, ft_strlen(token));
	write(2, "\'\n", 2);
	return (false);
}

static bool	check_combo(char const *first, char *second)
{
	if (first[0] == ';' && (second[0] == '>' || second[0] == '<'))
		return (true);
	if (first[0] == '|' && (second[0] == '<' || second[0] == '>'))
		return (true);
	return (syntax_error(second));
}

bool		syntax_check(t_list *tokenlist)
{
	t_list		*tmp;
	t_token		*current;
	t_token		*next;

	tmp = ft_lstlast(tokenlist);
	current = (t_token*)tmp->content;
	if (is_splitting(current) && current->string[0] != ';')
		return (syntax_error("newline"));
	tmp = tokenlist;
	current = (t_token*)tmp->content;
	if (is_splitting(current) && (current->string[0] == ';'
			|| (current->string[0] == '|')))
		return (syntax_error(current->string));
	while (tmp->next)
	{
		current = (t_token*)tmp->content;
		next = (t_token*)tmp->next->content;
		if (is_splitting(current) && is_splitting(next))
		{
			if (!check_combo(current->string, next->string))
				return (false);
		}
		tmp = tmp->next;
	}
	return (true);
}
