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

bool		is_redir(char c)
{
	if (c == redirect_input)
		return (true);
	if (c == redirect_append)
		return (true);
	if (c == redirect_trunc)
		return (true);
	return (false);
}

void		concat_list(t_list *tokenlist)
{
	t_list	*current;
	t_list	*next_elem;
	t_token	*current_token;
	t_token	*next_token;

	current = tokenlist->next;
	while (current)
	{
		current_token = (t_token*)current->content;
		if (!current_token->space_after && current->next)
		{
			next_elem = current->next;
			next_token = (t_token*)next_elem->content;
			if (is_redir(current_token->str[0]) || (next_token->end == ' ' &&
				(ft_strchr(";|<>", current_token->str[0])
				|| ft_strchr(";|<>", next_token->str[0]))))
			{
				current = current->next;
				continue ;
			}
			merge_tokens(current, next_elem);
		}
		else
			current = current->next;
	}
}

static void	set_redirects(char *str)
{
	if (str[0] == '<')
		str[0] = redirect_input;
	if (str[0] == '>' && !str[1])
		str[0] = redirect_trunc;
	if (str[0] == '>' && str[1] == '>')
	{
		str[0] = redirect_append;
		str[1] = 0;
	}
}

/*
**void	print_token_list(t_list *tokenlist)
**{
**	t_list	*tmp;
**	int i = 0;
**	tmp = tokenlist;
**	printf("         -|"); // to allign output
**	while (tmp)
**	{
**		unsetescape(((t_token*)tmp->content)->str);
** //		unset_redirects(((t_token*)tmp->content)->str);
**		printf("\033[0;36m");
**		if (i % 2)
**			printf("\033[0;31m");
**		printf("=%s=", ((t_token*)tmp->content)->str);
**		if (((t_token*)tmp->content)->space_after == true)
**			printf(" ");
**		tmp = tmp->next;
**		i++;
**	}
**	printf("\033[0m|-\n");
**}
*/

void		set_redirs(t_list *tokenlist)
{
	t_list	*tmp;
	t_token	*current;

	tmp = tokenlist->next;
	while (tmp)
	{
		current = (t_token*)tmp->content;
		if (current->end == ' ')
			set_redirects(current->str);
		tmp = tmp->next;
	}
}

void		addfakkingbefore(t_list *current, void *content)
{
	t_list	*new;

	new = ft_lstnew(content);
	malloc_check(new);
	if (!current->previous)
	{
		new->next = current;
		current->previous = new;
	}
	else
	{
		current->previous->next = new;
		new->previous = current->previous;
		new->next = current;
		current->previous = new;
	}
}

void		lol(void)
{
}

void		fakkingremovecurrent(t_list *current, void (*del)(void*))
{
	if (!current->next && !current->previous)
		lol();
	else if (!current->previous)
		current->next->previous = NULL;
	else if (!current->next)
		current->previous->next = NULL;
	else
	{
		current->previous->next = current->next;
		current->next->previous = current->previous;
	}
	del(current->content);
	free(current);
}

void		remove_escapes(t_list *tokenlist)
{
	t_list	*tmp;
	t_list	*tmp2;
	t_token	*current;
	char	old[2];

	tmp = tokenlist->next;
	old[0] = escape;
	old[1] = 0;
	while (tmp)
	{
		if (((t_token*)tmp->content)->end == '\"')
			unsetescape_if_literal(((t_token*)tmp->content)->str);
		((t_token*)tmp->content)->str =
				ft_replace(((t_token*)tmp->content)->str, old, "");
		malloc_check(((t_token*)tmp->content)->str);
		tmp2 = tmp->next;
		current = (t_token*)tmp->content;
		if (!ft_strcmp("", current->str) && current->end == ' ')
			fakkingremovecurrent(tmp, free_one_token);
		tmp = tmp2;
	}
}

bool		need_split(char *arg)
{
	int	i;

	i = 0;
	while (arg[i])
	{
		if (ft_isspace(arg[i]))
			return (true);
		i++;
	}
	return (false);
}

t_list		*split_current(t_list *tmp, const t_token *current)
{
	int		i;
	t_list	*tmp2;
	t_token	*new;
	char	**buff;

	buff = ft_split(current->str, ' ');
	i = 0;
	while (buff[i])
	{
		new = new_token(buff[i], ' ', ' ');
		malloc_check(new);
		addfakkingbefore(tmp, new);
		i++;
	}
	tmp2 = tmp->previous;
	fakkingremovecurrent(tmp, free_one_token);
	tmp = tmp2;
	return (tmp);
}

void		fakkkkkkkkk(t_list *tokenlist)
{
	t_list	*tmp;
	t_token	*current;

	tmp = tokenlist->next;
	while (tmp)
	{
		current = (t_token*)tmp->content;
		if (current->end == ' ' && need_split(current->str))
		{
			tmp = split_current(tmp, current);
		}
		tmp = tmp->next;
	}
}

void		do_everything(char *line)
{
	t_list	*tokenlist;
	t_list	*commands;

	if (!ft_strlen(line))
		return ;
	tokenlist = tokenizer(line);
	ft_lstadd_front(&tokenlist, ft_lstnew(new_token("start", '\'', 'a')));
	if (!tokenlist)
		return ;
	create_append(tokenlist);
	if (!syntax_check(tokenlist))
	{
		ft_lstclear(&tokenlist, free_one_token);
		return ;
	}
	set_redirs(tokenlist);
	expand_env_var(tokenlist);
	fakkkkkkkkk(tokenlist);
	concat_list(tokenlist);
	remove_escapes(tokenlist);
	commands = commandtokens(tokenlist);
	do_commands(commands);
	ft_lstclear(&commands, free_one_command);
}

void		prompt(void)
{
	write(2, g_shell.name, ft_strlen(g_shell.name));
	write(2, "$ ", 2);
}

void		handle_sig(int signal)
{
	char	deltwo[6];

	deltwo[0] = '\b';
	deltwo[1] = '\b';
	deltwo[2] = 127;
	deltwo[3] = 127;
	deltwo[4] = '\b';
	deltwo[5] = '\b';
	if (signal == SIGQUIT)
		write(1, deltwo, 6);
	if (signal == SIGINT)
	{
		g_shell.exitstatus = 1;
		write(1, deltwo, 6);
		write(1, "\n", 1);
		prompt();
	}
}

void		prep_global(char *const *av, char **envp)
{
	g_shell.envvars = malloc_vars(envp);
	g_shell.exitstatus = 0;
	g_shell.loopstatus = 1;
	g_shell.name = ft_substr(ft_strrchr(av[0], '/'), 1, ft_strlen(av[0]) - 1);
	malloc_check(g_shell.name);
	g_shell.og_stdout = dup(STDOUT_FILENO);
	g_shell.og_stdin = dup(STDIN_FILENO);
	if (g_shell.og_stdout == -1 || g_shell.og_stdin == -1)
		leaks_exit("error dupping original filestreams", -1);
	g_shell.is_child = false;
	signal(SIGQUIT, handle_sig);
	signal(SIGINT, handle_sig);
}

int			main(int ac, char **av, char **envp)
{
	char	*line;

	if (ac > 1)
		leaks_exit("Running with arguments is not supported.", 1);
	prep_global(av, envp);
	while (g_shell.loopstatus)
	{
		prompt();
		g_shell.loopstatus = get_next_line(0, &line);
		malloc_check(line);
		do_everything(line);
		free(line);
	}
	write(2, "exit\n", 5);
	exit(g_shell.exitstatus);
}
