cmake_minimum_required(VERSION 3.17)
project(Minishell C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g3 -Wall -Wextra -Werror ")
#                                       can add -fsanitize=address up here
set(CMAKE_LINKER_FLAGS)
add_executable(Minishell main.c tokenizer.c libft/ft_isspace.c env_vars.c err_and_exit.c token_aux.c syntax.c commands.c builtins.c env_aux_functions.c env_and_exit_builtins.c process_commands.c)

find_library(LIBFT libft.a libft)
target_link_libraries(Minishell LINK_PUBLIC ${LIBFT})

