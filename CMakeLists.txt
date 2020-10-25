cmake_minimum_required(VERSION 3.17)
project(Minishell C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -Werror")
#                                       can add -fsanitize=address up here
set(CMAKE_LINKER_FLAGS)
include_directories( ${incl})

add_executable(Minishell srcs/main.c srcs/tokenizer.c srcs/expand_env_vars.c srcs/err_and_exit.c srcs/escape_chars.c srcs/syntax.c srcs/commands.c srcs/builtins.c srcs/export_unset_aux_functions.c srcs/env_export_unset_builtins.c srcs/process_commands.c srcs/redirections.c srcs/executes.c srcs/pipes.c srcs/token_helper_functions.c srcs/list_helper_functions.c srcs/redir_chars.c srcs/split_tokens.c srcs/tokenizer_aux_functions.c srcs/prepwork.c srcs/expansion_aux_functions.c srcs/builtin_exit.c srcs/env_search_helper.c srcs/set_fds.c srcs/find_commands.c)

find_library(LIBFT libft.a libft)
target_link_libraries(Minishell LINK_PUBLIC ${LIBFT})

