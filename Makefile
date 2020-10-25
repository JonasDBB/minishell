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

FLAGS = -Wall -Wextra -Werror -g3 -fsanitize=address

SRCS = main.c \
tokenizer.c \
token_aux.c \
env_vars.c \
err_and_exit.c \
syntax.c \
commands.c \
builtins.c \
env_and_exit_builtins.c \
env_aux_functions.c \
process_commands.c \
executes.c \
redirections.c \
pieps.c

OBJS = $(SRCS:.c=.o)

all: $(NAME)

%.o: %.c
	gcc -c $< -o $@

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
