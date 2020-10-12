# **************************************************************************** #
#                                                                              #
#                                                         ::::::::             #
#    Makefile                                           :+:    :+:             #
#                                                      +:+                     #
#    By: jbennink <marvin@codam.nl>                   +#+                      #
#                                                    +#+                       #
#    Created: 2019/10/28 13:15:06 by jbennink      #+#    #+#                  #
#    Updated: 2020/08/07 15:13:27 by jbennink      ########   odam.nl          #
#                                                                              #
# **************************************************************************** #

NAME = libft.a

FLAGS = -Wall -Wextra -Werror -g

SRCS = ft_atoi.c ft_bzero.c ft_calloc.c ft_isalnum.c ft_isalpha.c ft_isascii.c \
ft_isdigit.c ft_isprint.c ft_itoa.c ft_memccpy.c ft_memchr.c ft_memcmp.c \
ft_memcpy.c ft_memmove.c ft_memset.c ft_putchar_fd.c ft_putendl_fd.c \
ft_putnbr_fd.c ft_putstr_fd.c ft_split.c ft_strchr.c ft_strdup.c ft_strjoin.c \
ft_strlcat.c ft_strlcpy.c ft_strlen.c ft_strmapi.c ft_strncmp.c ft_strnstr.c \
ft_strrchr.c ft_strtrim.c ft_substr.c ft_tolower.c ft_toupper.c \
get_next_line.c ft_strcmp.c \
ft_lstadd_back_bonus.c ft_lstadd_front_bonus.c \
ft_lstclear_bonus.c ft_lstdelone_bonus.c ft_lstiter_bonus.c \
ft_lstlast_bonus.c ft_lstmap_bonus.c ft_lstnew_bonus.c ft_lstsize_bonus.c \
ft_replace.c ft_isspace.c stupid_gnl_function.c

OBJS = $(SRCS:.c=.o)

.PHONY: all clean fclean re bonus

all: $(NAME)

%.o: %.c
	gcc $(FLAGS) -c $<

$(NAME): $(OBJS)
	ar rcs $(NAME) $?

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all
