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

NAME =			malloc
HEADER =		./includes/malloc.h
INC =			-I ./includes \
				-I ./libft/includes
LIB =			-lft -L ./libft
FLAGS =			#-Wall -Wextra -Werror
LIST =			malloc \
				testing \
				dump
OBJ =			$(addprefix obj/, $(addsuffix .o, $(LIST)))

ifeq ("$(DEBUG)", "1")
	DEBUGFLAG="-D DEBUG"
endif

OFF=\033[0m
PURPLE=\033[0;35m
PURPLEBOLD=\033[1;35m
WHITE=\033[1;37m
PURPLELIGHT=\033[38;2;102;102;255m

# TODO: libft_malloc_$HOSTTYPE.so
# Your Makefile will have to check the existence of the environment variable $HOSTTYPE. If it is empty or non-existant, to assign the following value:
# ‘uname -m‘_‘uname -s‘
# ifeq ($(HOSTTYPE),)
# HOSTTYPE := $(shell uname -m)_$(shell uname -s)
# endif
# • Your Makefile will have to create a symbolic link libft_malloc.so pointing to
# libft_malloc_$HOSTTYPE.so so for example:
# libft_malloc.so -> libft_malloc_intel-mac.so

# TODO: $> export HOSTTYPE = Тестирование
# ln -s libft_malloc_Testing.so libft_malloc.so
# $> ls -l libft_malloc.so
# libft_malloc.so -> libft_malloc_Testing.so
# $>
# Makefile хорошо использует HOSTTYPE, чтобы определить имя продавца книг
# (libft_malloc_ $ HOSSTYPE.so) и создает символическую ссылку libft_malloc.so, указывающую на
# libft_malloc_ $ HOSSTYPE.so?

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

all: $(NAME)

obj/%.o: src/%.c $(HEADER)
	@gcc $(FLAGS) -c $< -o $@ $(INC) $(DEBUGFLAG)
	@echo "$(PURPLELIGHT)Compiling $(WHITE)$< $(PURPLELIGHT)done$(OFF)"

$(NAME): ./libft/libft.a obj $(OBJ)
	@gcc $(OBJ) -o $(NAME) $(LIB)
	@echo "$(PURPLEBOLD)$(NAME)$(PURPLE) is ready$(OFF)"

./libft/libft.a:
	# TODO: uncomment
	# @make -C ./libft

obj:
	@mkdir obj

clean:
	@# @make -C ./libft clean
	@rm -rf obj
	
fclean: clean
	@# @make -C ./libft fclean
	@rm -f $(NAME)

re: fclean all
