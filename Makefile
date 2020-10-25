# **************************************************************************** #
#                                                                              #
#                                                         ::::::::             #
#    Makefile                                           :+:    :+:             #
#                                                      +:+                     #
#    By: jbennink <jbennink@student.codam.nl>         +#+                      #
#                                                    +#+                       #
#    Created: 2020/06/23 11:22:34 by jbennink      #+#    #+#                  #
#    Updated: 2020/07/08 13:03:22 by jbennink      ########   odam.nl          #
#                                                                              #
# **************************************************************************** #

NAME = Minishell

LIBFT = libft/libft.a

FLAGS = -Wall -Wextra -Werror

INCLUDES = incl/

_SRCS = builtin_exit.c \
builtins.c \
commands.c \
env_export_unset_builtins.c \
env_search_helper.c \
err_and_exit.c \
escape_chars.c \
executes.c \
expand_env_vars.c \
expansion_aux_functions.c \
export_unset_aux_functions.c \
find_commands.c \
list_helper_functions.c \
main.c \
pipes.c \
prepwork.c \
process_commands.c \
redir_chars.c \
redirections.c \
set_fds.c \
split_tokens.c \
syntax.c \
token_helper_functions.c \
tokenizer.c \
tokenizer_aux_functions.c

SRCS = $(addprefix srcs/, $(_SRCS))

OBJS = $(addprefix objects/, $(_SRCS:.c=.o))

all: $(NAME)

objects/%.o: srcs/%.c
	@mkdir -p objects
	gcc $(FLAGS) -I $(INCLUDES) -c $< -o $@

$(NAME): $(LIBFT) $(OBJS)
	gcc $(FLAGS) -o $(NAME) $(OBJS) $(LIBFT)

clean:
	rm -f $(OBJS)
	cd libft && $(MAKE) clean

fclean: clean
	rm -f $(NAME)
	cd libft && $(MAKE) fclean

re: fclean all

$(LIBFT):
	cd libft && $(MAKE) all

run: all
	./$(NAME)
