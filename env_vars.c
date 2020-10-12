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

static int	find_loc(char const *string, char c)
{
	int	i;

	i = 0;
	while (string[i] != c && string[i])
		i++;
	if (!string[i])
		return (-1);
	return (i + 1);
}

static char	*find_env(char *identifier)
{
	int		i;
	char	*before_eq;
	char	*after_eq;
	int		equals_loc;

	i = 0;
	while (g_shellvars.envvars[i])
	{
		equals_loc = find_loc(g_shellvars.envvars[i], '=');
		before_eq = ft_substr(g_shellvars.envvars[i], 0, equals_loc - 1);
		malloc_check(before_eq);
		after_eq = ft_substr(g_shellvars.envvars[i], equals_loc,
						ft_strlen(g_shellvars.envvars[i]) - equals_loc);
		malloc_check(after_eq);
		if (!ft_strcmp(identifier, before_eq))
		{
			free(before_eq);
			return (after_eq);
		}
		free(before_eq);
		free(after_eq);
		i++;
	}
	after_eq = ft_strdup("");
	malloc_check(after_eq);
	return (after_eq);
}

static char	*get_old_str(char *str, int loc)
{
	char	*res;
	int		i;

	i = 0;
	while (str[loc + i] != ' ' && str[loc + i] != '\t' && str[loc + i])
		i++;
	if (!i)
		res = ft_strdup("");
	else
		res = ft_substr(str, loc, i);
	malloc_check(res);
	return (res);
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

void		unset_signs(char *string)
{
	int	i;

	i = 1;
	while (string[i])
	{
		if (string[i] == '$' && string[i - 1] == escape)
			string[i] = temp_escaped;
	i++;
	}
}

void		expand_env_var(t_list *tokenlist)
{
	t_list		*tmp;
	t_tokens	*current;
	int			start_loc;
	char		*identifier;
	char		*old;
	char		*new;

	tmp = tokenlist;
	while (tmp)
	{
		current = (t_tokens*)tmp->content;
		unset_signs(current->string);
		if (!current->literal && ft_strchr(current->string, '$'))
		{
			start_loc = find_loc(current->string, '$');
			if (start_loc > 1)
			{
				if (current->string[start_loc - 2] == escape) {
					tmp = tmp->next;
					continue;
				}
			}
			identifier = get_old_str(current->string, start_loc);
			malloc_check(identifier);
			new = find_env(identifier);
			old = ft_strjoin("$", identifier);
			malloc_check(old);
			free(identifier);
			current->string = ft_replace(current->string, old, new);
			free(old);
			free(new);
			reset_signs(current->string);
		}
		else
		{
			reset_signs(current->string);
			tmp = tmp->next;
		}
	}
}
