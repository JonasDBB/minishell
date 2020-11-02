/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   env_vars.c                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: jbennink <jbennink@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/10/09 08:17:57 by jbennink      #+#    #+#                 */
/*   Updated: 2020/10/09 08:17:57 by jbennink      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../incl/minishell.h"

char			*find_env(char *identifier)
{
	int		i;
	char	*before;
	char	*after;
	int		loc;

	i = 0;
	while (g_shell.envvars[i])
	{
		loc = find_loc(g_shell.envvars[i], '=');
		if (loc == -1)
			break ;
		split_string(i, loc, &before, &after);
		if (!ft_strcmp(identifier, before))
		{
			free(before);
			return (after);
		}
		free(before);
		free(after);
		i++;
	}
	after = ft_strdup("");
	malloc_check(after);
	return (after);
}

static void		replace_env_var(t_token *current, int start_loc)
{
	char	*identifier;
	char	*old;
	char	*new;

	identifier = get_old_str(current->str, start_loc);
	new = find_env(identifier);
	old = ft_strjoin("$", identifier);
	malloc_check(old);
	free(identifier);
	current->str = ft_replace(current->str, old, new);
	malloc_check(current->str);
	free(old);
	free(new);
	reset_signs(current->str);
}

static t_token	*prep_token(const t_list *tmp, t_token *current)
{
	char	*exit_status;

	exit_status = ft_itoa(g_shell.exitstatus);
	malloc_check(exit_status);
	current = (t_token*)tmp->content;
	unset_signs(current->str);
	current->str = ft_replace(current->str, "$?", exit_status);
	malloc_check(current->str);
	free(exit_status);
	return (current);
}

static bool		is_end_of_command(t_token *token)
{
	if (token->end != ' ')
		return (false);
	if (!ft_strchr(";", token->str[0]))
		return (false);
	if (!token->str[0])
		return (false);
	return (true);
}

void			expand_env_var(t_list *tokenlist)
{
	t_list	*tmp;
	t_token	*cur;
	int		start;

	tmp = tokenlist;
	while (tmp && !is_end_of_command((t_token*)tmp->content))
	{
		cur = prep_token(tmp, cur);
		if (!cur->literal && ft_strchr(cur->str, '$'))
		{
			start = find_loc(cur->str, '$');
			if (start > 1)
				if (cur->str[start - 2] == escape || cur->str[start] == escape)
				{
					tmp = tmp->next;
					continue;
				}
			replace_env_var(cur, start);
		}
		else
		{
			reset_signs(cur->str);
			tmp = tmp->next;
		}
	}
}
