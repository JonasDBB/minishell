/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   echo.h                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: jbennink <jbennink@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/06/25 14:37:31 by jbennink      #+#    #+#                 */
/*   Updated: 2020/07/08 13:27:23 by jbennink      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef ECHO_H
# define ECHO_H

# include "minishell.h"

typedef struct 	s_shit {
	int		start;
	int		len;
	char	literal;
	char	space;
	char	*content;
}				t_shit;

#endif
