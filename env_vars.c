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

#include "minishell.h"

char		**malloc_vars(char **envp)
{
	char	**result;
	int		count;

	count = 0;
	while (envp[count])
		count++;
	result = malloc(sizeof(char*) * (count + 1));
	malloc_check(result);
	count = 0;
	while (envp[count])
	{
		result[count] = ft_strdup(envp[count]);
		malloc_check(result[count]);
		count++;
	}
	result[count] = NULL;
	return (result);
}

int			find_loc(char const *string, char c)
{
	int	i;

	i = 0;
	while (string[i] != c && string[i])
		i++;
	if (!string[i])
		return (-1);
	return (i + 1);
}

void		split_string(int i, int loc, char **before, char **after)
{
	(*before) = ft_substr(g_shell.envvars[i], 0, loc - 1);
	malloc_check((*before));
	(*after) = ft_substr(g_shell.envvars[i], loc,
						ft_strlen(g_shell.envvars[i]) - loc);
	malloc_check((*after));
}

char		*find_env(char *identifier)
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

static char	*get_old_str(char *str, int loc)
{
	char	*res;
	int		i;

	i = 0;
	if (str[loc] == '_' || ft_isalpha(str[loc]))
	{
		while ((ft_isalnum(str[loc + i]) || str[loc + i] == '_') &&
				str[loc + i])
		{
			i++;
		}
	}
	if (!i)
		res = ft_strdup("");
	else
		res = ft_substr(str, loc, i);
	malloc_check(res);
	return (res);
}

void		unset_signs(char *string)
{
	int	i;

	i = 1;
	if (!string[0])
		return ;
	while (string[i])
	{
		if (string[i] == '$' && string[i - 1] == escape)
			string[i] = temp_escaped;
		i++;
	}
}

void		reset_signs(char *string)
{
	int	i;

	i = 0;
	while (string[i])
	{
		if (string[i] == temp_escaped)
			string[i] = '$';
		i++;
	}
}

void		replace_env_var(t_token *current, int start_loc)
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

t_token		*prep_token(const t_list *tmp, t_token *current)
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

void		expand_env_var(t_list *tokenlist)
{
	t_list	*tmp;
	t_token	*cur;
	int		start;

	tmp = tokenlist->next;
	while (tmp)
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
