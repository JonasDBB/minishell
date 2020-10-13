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

static char	*strjoin_and_free(char *s1, char *s2)
{
	char	*result;
	char	*tmp;

	if (s1[0])
		tmp = ft_strjoin(s1, " ");
	else
		tmp = ft_strdup(s1);
	malloc_check(tmp);
	result = ft_strjoin(tmp, s2);
	malloc_check(result);
	free(s1);
	free(tmp);
	return (result);
}

char	**create_commands(t_list *tokenlist)
{
	int		count;
	t_list	*tmp;
	char	**result;
	char	*command;
	int		i;

	count = 1;
	tmp = tokenlist;
	while(tmp)
	{
		if (is_splitting((t_tokens*)tmp->content))
			count += 2;
		if (is_splitting((t_tokens*)tmp->content) && !tmp->next)
			count--;
		tmp = tmp->next;
	}
	result = malloc(sizeof(char*) * (count + 1));
	malloc_check(result);
	result[count] = NULL;
	i = 0;
	tmp = tokenlist;
	while (tmp)
	{
		command = ft_strdup("");
		malloc_check(command);
		while (!is_splitting((t_tokens*)tmp->content))
		{
			command = strjoin_and_free(command, ((t_tokens*)tmp->content)->string);
			if (!tmp->next)
				break;
			tmp = tmp->next;
		}
		result[i] = command;
		i++;
		if (is_splitting((t_tokens*)tmp->content)) {
			result[i] = ft_strdup(((t_tokens*)tmp->content)->string);
			i++;
		}
		tmp = tmp->next;
	}
	if (i < count)
		result[i] = NULL;
	return (result);
}

void	free_array(char **array)
{
	int	i;

	i = 0;
	while (array[i])
	{
		free(array[i]);
		i++;
	}
	free(array);
}
