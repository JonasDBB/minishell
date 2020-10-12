/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   minishell.h                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: jbennink <jbennink@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/06/23 09:54:21 by jbennink      #+#    #+#                 */
/*   Updated: 2020/06/25 14:43:44 by jbennink      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "libft/libft.h"
# include "echo.h"
# include <stdlib.h>
# include <fcntl.h>
# include <stdbool.h>
# include <signal.h>

typedef struct	s_shellvars {
	char	**envvars;
	int 	loopstatus;
	int 	exitstatus;
	char 	*name;
}				t_shellvars;

t_shellvars	g_shellvars;

enum			e_state
{
	escape = -1
};

typedef struct	s_tokens {
	char	*string;
	char 	literal;
	char 	end;
	char 	space_after;
}				t_tokens;

void	print_token_list(t_list *tknlist);
bool	syntax_check(t_list *tokenlist);

t_list	*tokenizer(char *inputline);

/*
** env_vars.c
*/
char	**malloc_vars(char **envp);
void	expand_env_var(t_list *tokenlist);

/*
** err_and_exit.c
*/
void	malloc_check(void *p);

void	free_one_token(void *token);
void	leaks_exit(char *error, int exitcode);

/*
** token_aux.c
*/
void	merge_tokens(t_list *current, t_list *next_elem);
void	create_append(t_list *tokenlist);
void	setescape(char *s);
void	unsetescape(char *s);

#endif
