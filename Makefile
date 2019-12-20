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
FLAGS =			-fPIC #-Wall -Wextra -Werror
LIST =			malloc \
				testing \
				dump \
				mmap
OBJ =			$(addprefix obj/, $(addsuffix .o, $(LIST)))
SRC =			$(addprefix src/, $(addsuffix .c, $(LIST)))


OFF=\033[0m
PURPLE=\033[0;35m
PURPLEBOLD=\033[1;35m
WHITE=\033[1;37m
PURPLELIGHT=\033[38;2;102;102;255m

ifeq ("$(DEBUG)", "1")
	DEBUGFLAG="-D DEBUG"
endif

# TODO: $> export HOSTTYPE = Тестирование
# ln -s libft_malloc_Testing.so libft_malloc.so
# $> ls -l libft_malloc.so
# libft_malloc.so -> libft_malloc_Testing.so

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

all: $(LINK_NAME)

obj/%.o: src/%.c $(HEADER)
	@clang $(FLAGS) -c $< -o $@ $(INC) $(DEBUGFLAG)
	@echo "$(PURPLELIGHT)Compiling $(WHITE)$< $(PURPLELIGHT)done$(OFF)"

$(NAME): ./libft/libft.a obj $(OBJ)
	@clang -shared -fPIC $(OBJ) -o $(NAME)
	@echo "$(PURPLEBOLD)$(NAME)$(PURPLE) ready$(OFF)"

$(LINK_NAME): $(NAME)
	@ln -sf $(NAME) $(LINK_NAME)
	@echo "$(PURPLEBOLD)$(LINK_NAME)$(PURPLE) linked$(OFF)"

./libft/libft.a:
	# TODO: uncomment
	# @make -C ./libft

obj:
	@mkdir obj

clean:
	@# @make -C ./libft clean TODO: uncomment
	@rm -rf obj
	
fclean: clean
	@# @make -C ./libft fclean TODO: uncomment
	@rm -f $(NAME) $(LINK_NAME)

re: fclean all
