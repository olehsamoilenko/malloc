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

all: $(LINK_NAME) tests

obj/%.o: src/%.c $(HEADER)
	@gcc $(FLAGS) -c $< -o $@ $(INC) $(DEBUGFLAG)
	@echo "$(PURPLELIGHT)Compiling $(WHITEBOLD)$< $(PURPLELIGHT)done$(OFF)"

$(LINK_NAME): $(NAME)
	@ln -sf $(NAME) $(LINK_NAME)
	@echo "$(PURPLEBOLD)$(LINK_NAME)$(PURPLE) linked$(OFF)"

$(NAME): ./libft/libft.a obj $(OBJ) # TODO: move obj to obj/%.o
	@gcc -shared $(FLAGS) $(OBJ) ./libft/libft.a -o $(NAME)
	@strip -x $(NAME)
	@echo "$(PURPLEBOLD)$(NAME)$(PURPLE) ready$(OFF)"

./libft/libft.a:
	@make -C ./libft

obj:
	@mkdir obj

clean:
	@rm -rf obj # TODO: console log
	
fclean: clean
	@rm -f $(NAME) $(LINK_NAME) test0 test1 test2 # TODO: refactor $(TESTS)

re: fclean all

tests: $(LINK_NAME) test_0 # TODO: console log

test_0:
	# To run tests:
	@gcc -o test0 tests/test0.c
	# ./run.sh && /usr/bin/time -l ./test0
	@gcc -o test1 tests/test1.c
	# ./run.sh && /usr/bin/time -l ./test1
	@gcc -o test2 tests/test2.c
	# ./run.sh && /usr/bin/time -l ./test2
	@#gcc -o test3 tests/test3.c # TODO: uncomment when realloc is done
	@# ./run.sh && ./test3
	@#gcc -o test3b tests/test3b.c # TODO: uncomment when realloc is done
	@# ./run.sh && ./test3b
	@#gcc -o test4 tests/test4.c # TODO: uncomment when realloc is done
	@# ./run.sh && ./test4
	@gcc $(INC) tests/test9.c -o test9 -lft -L libft -lft_malloc -L .
	@# TODO: test5
	@# TODO: bonuses
