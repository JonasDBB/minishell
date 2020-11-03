/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   minishell.h                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: jbennink <jbennink@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/06/23 09:54:21 by jbennink      #+#    #+#                 */
/*   Updated: 2020/11/03 21:45:40 by jonasbb       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../libft/libft.h"
# include <stdlib.h>
# include <fcntl.h>
# include <stdbool.h>
# include <signal.h>
# include <sys/stat.h>
# include <errno.h>
# include <string.h>
# include <sys/wait.h>

enum			e_state
{
	last,
	redirect_trunc,
	redirect_append,
	redirect_input,
	escape = -1,
	temp_escaped = -2
};

typedef struct	s_shellvars {
	char			**envvars;
	int				loopstatus;
	unsigned char	exitstatus;
	char			*name;
	int				og_stdout;
	int				og_stdin;
	bool			is_child;
}				t_shellvars;

typedef struct	s_token {
	char	*str;
	bool	literal;
	char	end;
	bool	space_after;
}				t_token;

typedef struct	s_command {
	char	**tokens;
	char	type;
	int		pipe_fds[2];
}				t_command;

typedef struct	s_pipe_pids {
	pid_t	*pids;
	int		size;
}				t_pipe_pids;

t_shellvars	g_shell;

/*
** builtin_exit.c
*/
void			builtin_exit(char **args);

/*
** builtins.c
*/
void			builtin_pwd(void);
void			builtin_echo(char **args);
void			builtin_cd(char **args);

/*
** commands.c
*/
void			commandtokens(t_list *tokenlist);

/*
** env_export_unset_builtins.c
*/
void			builtin_env(char *arg);
void			builtin_export(char **args);
void			remove_one_env_var(int index_to_remove);
void			builtin_unset(char **args);

/*
** env_search_helper.c
*/
int				find_env_loc(char *arg);
bool			remove_if_exists(char *envvar);

/*
** err_and_exit.c
*/
void			malloc_check(void *p);
void			free_array(char **array);
void			free_one_command(void *token);
void			free_one_token(void *token);
void			leaks_exit(char *error, int exitcode);

/*
** escape_chars.c
*/
void			remove_escapes(t_list **tokenlist);
void			setescape(char *s);
void			unsetescape(char *s);

/*
** executes.c
*/
void			exec(char **args, char *exec_path);
bool			find_executables(char **args);

/*
** expand_env_vars.c
*/
char			*find_env(char *identifier);
void			expand_env_var(t_list *tokenlist);

/*
** expansion_aux_functions.c
*/
int				find_loc(char const *string, char c);
void			split_string(int i, int loc, char **before, char **after);
char			*get_old_str(char *str, int loc);
void			unset_signs(char *string);
void			reset_signs(char *string);

/*
** export_unset_aux_functions.c
*/
bool			env_check(char *arg);
bool			env_check_unset(char *arg);
void			print_one_env(char *var, int i);
void			print_env_args(void);

/*
** find_commands.c
*/
void			find_command(t_command *current);

/*
** list_helper_functions.c
*/
void			lst_remove_current(t_list *current, void (*del)(void*));
void			lst_add_before(t_list *current, void *content);
void			concat_list(t_list *tokenlist);

/*
** main.c
*/
void			handle_sig(int signal);

/*
** pipes.c
*/
void			set_exit_from_child(int status);
void			make_pipe(t_command *current, t_command *previous, pid_t *pids);

/*
** prepwork.c
*/
char			**malloc_vars(char **envp);
void			prep_global(char *const *av, char **envp);

/*
** process_commands.c
*/
void			do_one_command(t_list *tmp, t_pipe_pids *pipe_pids);
void			do_command_list(t_list *commandlist);

/*
** redir_chars.c
*/
void			set_redirects(char *str);
void			set_redirs(t_list *tokenlist);
bool			is_redir(char c);
void			create_append(t_list *tokenlist);

/*
** redirections.c
*/
bool			find_redirects(char **tokens);

/*
** set_fds.c
*/
int				set_fd_in(char *file, int old_fd);
int				set_fd_out(char *file, int flag, int old_fd);
void			setfds(int fd_new, int std_fd);
void			reset_fds(void);

/*
** split_tokens.c
*/
void			find_breaks(t_list **tokenlist);

/*
** syntax.c
*/
bool			is_splitting(t_token *token);
bool			syntax_check(t_list *tokenlist);

/*
** token_helper_functions.c
*/
t_token			*new_token(char *content, char end, char space_after);
void			retokenize_expanded_vars(t_list *tokenlist);
void			merge_tokens(t_list *current, t_list *next_elem);

/*
** tokenizer.c
*/
t_list			*tokenizer(char *inputline);

/*
** tokenizer_aux_functions.c
*/
int				cmp_end_char(char c, char prev, char stop);
int				is_splitting_char(char c, char prev);
int				get_token_len(char *str, char stop, int it);

#endif
