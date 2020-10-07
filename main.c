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

t_shellvars	g_shellvars;

void 		malloc_check(void *p)
{
	if (!p)
		leaks_exit("malloc2 fail", 1);
}

void		leaks_exit(char *error, int exitcode)
{
	char 	*tmp;
	char	*command;

	if (exitcode)
	{
		write(2, "error: ",7);
		write(2, error, ft_strlen(error));
		write(2, "\n\n", 2);
	}
	tmp = ft_strjoin("leaks \"", g_shellvars.name);
	malloc_check(tmp);
	command = ft_strjoin(tmp, "\" >leaks.txt");
	malloc_check(command);
	free(tmp);
	system(command);
	free(command);
//	system("cat leaks.txt"); // for full display of leaks; comment out line below to use
	system("grep \"total leaked bytes\" leaks.txt"); // for short leaks; comment out line above
	system("rm leaks.txt");
	exit(exitcode);
}

void		concat_list(t_list *tokenlist)
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

			// check if either token is a splitting character
			if (ft_strchr(";|<>", current_token->string[0]) || ft_strchr(";|<>", next_token->string[0]))
			{
				current = current->next;
				continue ;
			}

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

// env list as linked tokens
//void		expand_envp(t_list *tokenlist, char **envp)
//{
//	int i = 0;
//	(void)tokenlist;
//	while (envp[i])
//	{
//		printf("-|%s|-\n", envp[i]);
//		i++;
//	}

//}

//t_env		*envtoken(char *line)
//{
//	t_env	*result;
//	int 	i;
//	char 	*before;
//	char 	*after;
//	int 	equals;
//
//	i = 0;
//	result = malloc(sizeof(t_env));
//	while (line[i] != '=')
//		i++;
//	equals = i;
//	before = malloc(i + 1);
//	i = 0;
//	while (i < equals)
//	{
//		before[i] = line[i];
//		i++;
//	}
//	while (line[i])
//		i++;
//	after = malloc(i);
//	i = equals + 1;
//	while (line[i])
//	{
//		after[i - (equals + 1)] = line[i];
//		i++;
//	}
//	result->identifier = before;
//	result->literal = after;
//	return (result);
//}

//t_list		*envlister(char **envp)
//{
//	t_list	*envlist;
//	int 	i;
//
//	i = 0;
//	envlist = NULL;
//	while (envp[i])
//	{
//		ft_lstadd_back(&envlist, ft_lstnew(envtoken(envp[i])));
//		i++;
//	}
//	return (envlist);
//}

//void 		print_env_list(t_list *envlist)
//{
//	t_list	*tmp;
//	int i = 0;
//
//	tmp = envlist;
//	printf("\n");
//	while (tmp)
//	{
//		printf("\033[0;36m");
//		if (i % 2)
//			printf("\033[0;31m");
//		printf("%s = %s\n", ((t_env*)tmp->content)->identifier, ((t_env*)tmp->content)->literal);
//		tmp = tmp->next;
//		i++;
//	}
//	printf("\033[0m|-\n");
//}

void 		print_token_list(t_list *tknlist)
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

char 		**malloc_vars(char **envp)
{
	char	**result;
	int		count;

	count = 0;
	while (envp[count])
		count++;
	result = malloc(sizeof(char*) * count);
	malloc_check(result);
	count = 0;
	while (envp[count])
	{
		result[count] = ft_strdup(envp[count]);
		malloc_check(result[count]);
		count++;
	}
	return (result);
}

void		do_everything(char *line)
{
	t_list	*tknlist;
//	t_list	*envlist;

//	envlist = envlister(envp);
//	print_env_list(envlist);
	if (!ft_strlen(line))
		return ;
	tknlist = tokenizer(line);
	if (!tknlist)
		return ;
//	do expansions
//	expand_envp(tknlist, envp);
	concat_list(tknlist);
	print_token_list(tknlist);
	if (!ft_strcmp("exit", ((t_tokens*)tknlist->content)->string))
		leaks_exit("", 0);
	ft_lstclear(&tknlist, free_one_token);
}

int			main(int ac, char **av, char **envp)
{
	char	*line;

	if (ac > 1) {
		leaks_exit("Running with arguments is not supported.", 1);
	}
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
