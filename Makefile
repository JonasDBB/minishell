# **************************************************************************** #
#                                                                              #
#                                                         ::::::::             #
#    Makefile                                           :+:    :+:             #
#                                                      +:+                     #
#    By: jbennink <jbennink@student.codam.nl>         +#+                      #
#                                                    +#+                       #
#    Created: 2020/06/23 11:22:34 by jbennink      #+#    #+#                  #
#    Updated: 2020/06/24 13:20:41 by jbennink      ########   odam.nl          #
#                                                                              #
# **************************************************************************** #

NAME = Minishell

LIBFT = libft/libft.a

FLAGS = -Wall -Wextra -Werror

SRCS = main0.c \
echo.c

OBJS = $(SRCS:.c=.o)

all: $(NAME)

%.o: %.c
	gcc $(FLAGS) -c $< -o $@

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