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
# include <stdlib.h>
# include <fcntl.h>
# include <stdbool.h>
# include <signal.h>

typedef struct	s_shellvars {
	char			**envvars;
	int 			loopstatus;
	unsigned char 	exitstatus;
	char	 		*name;
	char			*exitmessage;
}				t_shellvars;

t_shellvars	g_shellvars;

enum			e_state
{
	end,
	escape = -1,
	temp_escaped = 48
};

typedef struct	s_token {
	char	*string;
	bool 	literal;
	char 	end;
	bool 	space_after;
}				t_token;

typedef struct	s_command {
	char				**tokens;
	char				type;
}				t_command;

/*
** main.c
*/
void	print_token_list(t_list *tokenlist);
t_list	*tokenizer(char *inputline);


/*
** builtins.c
*/
void	do_commands(t_list *commandlist);

/*
** commands.c
*/
t_list	*commandtokens(t_list *tokenlist);

/*
** env_vars.c
*/
char	**malloc_vars(char **envp);
void	expand_env_var(t_list *tokenlist);

/*
** err_and_exit.c
*/
void	malloc_check(void *p);
void	free_array(char **array);
void	free_one_command(void *token);
void	free_one_token(void *token);
void	leaks_exit(char *error, int exitcode);

/*
** token_aux.c
*/
void	merge_tokens(t_list *current, t_list *next_elem);
void	create_append(t_list *tokenlist);
void	setescape(char *s);
void	unsetescape(char *s);

/*
** syntax.c
*/
bool	syntax_check(t_list *tokenlist);
bool	is_splitting(t_token *token);

#endif
