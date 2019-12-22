# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: osamoile <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2018/08/01 15:53:40 by osamoile          #+#    #+#              #
#    Updated: 2019/11/09 16:15:32 by osamoile         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

ifeq ($(HOSTTYPE),)
	HOSTTYPE := $(shell uname -m)_$(shell uname -s)
endif

NAME =			libft_malloc_$(HOSTTYPE).so
LINK_NAME =		libft_malloc.so
HEADER =		./includes/malloc.h
INC =			-I ./includes \
				-I ./libft/includes
LIB =			-lft -L ./libft
FLAGS =			-fvisibility=hidden # -fPIC # TODO: -Wall -Wextra -Werror
LIST =			malloc \
				dump \
				mmap
OBJ =			$(addprefix obj/, $(addsuffix .o, $(LIST)))
SRC =			$(addprefix src/, $(addsuffix .c, $(LIST)))


OFF=\033[0m
PURPLE=\033[0;35m
PURPLEBOLD=\033[1;35m
WHITEBOLD=\033[1;37m
PURPLELIGHT=\033[38;2;102;102;255m

ifeq ("$(DEBUG)", "1")
	DEBUGFLAG="-D DEBUG"
endif

# TODO:
# cat run.sh #!/bin/sh export DYLD_LIBRARY_PATH=. export
# DYLD_INSERT_LIBRARIES="libft_malloc.so" export DYLD_FORCE_FLAT_NAMESPACE=1 $@

# TODO: $> nm libft_malloc.so
# 0000000000000000 T _free
# 0000000000000000 T _malloc
# 0000000000000000 T _realloc
# 0000000000000000 T _show_alloc_mem
# U _mmap
# U _munmap
# U _getpagesize
# U _write
# U dyld_stub_binder

all: $(LINK_NAME) tests

obj/%.o: src/%.c $(HEADER)
	@gcc $(FLAGS) -c $< -o $@ $(INC) $(DEBUGFLAG)
	@echo "$(PURPLELIGHT)Compiling $(WHITEBOLD)$< $(PURPLELIGHT)done$(OFF)"

$(NAME): ./libft/libft.a obj $(OBJ)
	@gcc -shared $(FLAGS) $(OBJ) ./libft/libft.a -o $(NAME)
	@strip -x $(NAME)
	@echo "$(PURPLEBOLD)$(NAME)$(PURPLE) ready$(OFF)"

$(LINK_NAME): $(NAME)
	@ln -sf $(NAME) $(LINK_NAME)
	@echo "$(PURPLEBOLD)$(LINK_NAME)$(PURPLE) linked$(OFF)"

./libft/libft.a:
	@make -C ./libft # TODO: remove stdlib libft, check memalloc

obj:
	@mkdir obj

clean:
	@rm -rf obj # TODO: console log
	
fclean: clean
	@rm -f $(NAME) $(LINK_NAME) test_0 # TODO: refactor

re: fclean all

tests: $(LINK_NAME) test_0 # TODO: console log

test_0:
	@gcc $(INC) tests/test_0.c -o test_0 -lft -L libft -lft_malloc -L .
