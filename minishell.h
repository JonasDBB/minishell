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

typedef struct	s_shellvars {
	char	**envvars;
	int 	loopstatus;
	int 	exitstatus;
	char 	*name;
}				t_shellvars;

enum			e_num
{
	escape = -1,
	false = 0,
	true = 1
};

typedef struct	s_tokens {
	char	*string;
	char 	literal;
	char 	end;
	char 	space_after;
}				t_tokens;

void	setescape(char *s);
void	unsetescape(char *s);


void	print_token_list(t_list *tknlist);
void	leaks_exit(char *error, int exitcode);
void	free_one_token(void *token);
t_list	*tokenizer(char *inputline);


void 		malloc_check(void *p);

#endif
